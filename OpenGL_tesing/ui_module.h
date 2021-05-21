#include <ft2build.h>
#include FT_FREETYPE_H

// Character structure.
struct character {
	// Texture id of glyph.
	unsigned int texture_id;
	// Size of glyph.
	glm::ivec2 size; 
	// Offset from line from font's line to upper left corner of glyph.
	glm::ivec2 bearing; 
	// Offset to next glyph.
	unsigned int advance;
};

#pragma once
class ui_module
{
public:
	// Deffault font size.
	const int DEFAULT_FONT_SIZE = 48;
	// Initialize module with font.
	ui_module(const char* text_font);
	// Returns font size.
	int get_font_size() { return font_size; }
	// Sets font size.
	void set_font_size(const int size) 
	{ 
		if (size >= 0)
			font_size = size;
		else
			throw new std::runtime_error("Font size can't be negative.");
		on_font_size_changed();
	}
	// Loads characters from font file.
	void load_characters();
	// Renders text on scene.
	void render_text(shader& text_shader, std::string text, float x, float y, float scale, glm::vec3 color);
	// Clears and destroys all module's buffers. 
	void terminate();
private:
	// FreeType library.
	FT_Library ft;
	// FreeType face.
	FT_Face face;
	// VAO of text quad.
	unsigned int text_VAO;
	// VBO of text quad.
	unsigned int text_VBO;
	// Map of all loaded charactes.
	std::map<char, character> characters;
	// Current font size.
	int font_size = DEFAULT_FONT_SIZE;
	// Initialize characters from font.
	int init(const char* text_font);
	// Runs when font size changed.
	void on_font_size_changed();
	// Generates buffers for text.
	void gen_buffers();
};

