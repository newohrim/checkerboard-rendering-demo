#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "shader.cpp"
#include "ui_module.h"

void ui_module::render_text(shader& text_shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Активируем соответствующее состояние рендеринга	
    text_shader.use();
    glUniform3f(glGetUniformLocation(text_shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);

    // Перебираем все символы
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        character ch = characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Обновляем VBO для каждого символа
        float vertices[6][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },

            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        // Визуализируем текстуру глифа поверх прямоугольника
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        // Обновляем содержимое памяти VBO
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Рендерим прямоугольник
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Теперь производим смещение для отображения следующего глифа (обратите внимание, что данное смещение измеряется в единицах, составляющих 1/64 пикселя)
        x += (ch.advance >> 6)* scale; // побитовый сдвиг на 6, чтобы получить значение в пикселях (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ui_module::terminate()
{
    for (auto& c : characters)
        glDeleteTextures(1, &c.second.texture_id);
    glDeleteVertexArrays(1, &text_VAO);
    glDeleteBuffers(1, &text_VBO);
}

int ui_module::init(const char* text_font)
{
    //FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return EXIT_FAILURE;
    }
    //FT_Face face;
    if (FT_New_Face(ft, text_font, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return EXIT_FAILURE;
    }
    //load();
    //gen_buffers();

    FT_Set_Pixel_Sizes(face, 0, font_size);
    return EXIT_SUCCESS;
}

void ui_module::load_characters()
{
    gen_buffers();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // отключаем ограничение выравнивания байтов

    for (unsigned int i = 0; i < 256; i++)
    {
        // Загружаем глифы символов 
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Генерируем текстуру
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Задаем для текстуры необходимые опции
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Сохраняем символ для последующего использования
        character c = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<char, character>(i, c));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Освобождение ресурсов
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void ui_module::on_font_size_changed()
{
    FT_Set_Pixel_Sizes(face, 0, font_size);
}

void ui_module::gen_buffers()
{
    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ui_module::ui_module(const char* text_font)
{
	init(text_font);
}
