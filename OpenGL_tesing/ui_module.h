#include <ft2build.h>
#include FT_FREETYPE_H

struct character {
	unsigned int texture_id; // ID текстуры глифа
	glm::ivec2 size; // размер глифа
	glm::ivec2 bearing; // смещение от линии шрифта до верхнего/левого угла глифа
	unsigned int advance; // смещение до следующего глифа
};

#pragma once
class ui_module
{
public:
	const int DEFAULT_FONT_SIZE = 48;
	ui_module(const char* text_font);
	int get_font_size() { return font_size; }
	void set_font_size(const int size) 
	{ 
		if (size >= 0)
			font_size = size;
		else
			throw new std::runtime_error("Font size can't be negative.");
		on_font_size_changed();
	}
	void load_characters();
	void render_text(shader& text_shader, std::string text, float x, float y, float scale, glm::vec3 color);
	void terminate();
private:
	FT_Library ft;
	FT_Face face;
	unsigned int text_VAO;
	unsigned int text_VBO;
	std::map<char, character> characters;
	int font_size = DEFAULT_FONT_SIZE;
	int init(const char* text_font);
	void on_font_size_changed();
	void gen_buffers();
};

