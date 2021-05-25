#include <glad/glad.h>
#include <iostream>
#include "shader.cpp"

#pragma once
class chb_fbo
{
private:
	// OpenGL backbuffer id.
	const unsigned int DEFFAULT_FRAMEBUFFER = 0;
	// Path to screen quad vertex shader.
	const char* PATH_TO_VERTEXSHADER = "shaders/fbo_to_screenquad_vertex_shader.glsl";
	// Path to screen quad fragment shader with no checkerboard support.
	const char* PATH_TO_SIMPLEFRAGMENTSHADER = "shaders/fbo_to_screenquad_simple_shader.glsl";
	// Path to screen quad fragment shader with checkerboard support.
	const char* PATH_TO_CHBFRAGMENTSHADER = "shaders/fbo_to_screenquad_fragment_shader.glsl";
	// Path to screen quad fragment shader with checkerboard interpolation support.
	const char* PATH_TO_CHBFRAGMENTSHADERINTERPOLATED = "shaders/fbo_to_screenquad_fragment_shader_interpolated.glsl";
	// Current chb framebuffer object id.
	unsigned int fbo_id;
	// Framebuffer width.
	int fbo_width;
	// Framebuffer height.
	int fbo_height;
	// Generated checkerboard data for depth/stencil texture.
	int* checkerboard;
	unsigned int checkerboard_tex;
	// Even screen texture id.
	unsigned int screen_texeven;
	// Odd screen texture id.
	unsigned int screen_texodd;
	// Screen quad VBO id.
	unsigned int screen_quadVBO;
	// Screen quad VAO id.
	unsigned int screen_quadVAO;
	// GL_LINEAR screen textures mode.
	const bool filter_linear = true;
	// Represent if current frame odd.
	bool is_oddframe = false;
	// Represent if checkerboard rendering is active.
	bool chb_active = true;
	// Count of fragments which color values will be interpolated in resulted color.
	const int interpolation_count = 0;
	// Screen quad shader with no checkerboard R support.
	shader* simple_screenquadshader;
	// Screen quad shader with checkerboard R support.
	shader* chb_screenquadshader;
	// Creates framebuffer.
	void create_fbo();
	// Creates depth/stencil texture based on data generated by getCheckerboardPattern.
	void create_chbtex();
	// Creates screen even and odd textures.
	void create_screentex();
	// Creates VBO and VAO for screenquad if necessary.
	void create_screenquad();
	// Add color and depth/stencil attachments to framebuffer.
	void set_attachments();
	// Checks if framebuffer is complete.
	void fbo_checkerror();
	// Checks if there was an error in last OpenGL command.
	void getError();

	// Vertices for screen quad.
	float quad_vertices[24] = { 
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
public:
	// Returns framebuffer's id.
	unsigned int get_id();
	// Activates framebuffer.
	void use();
	// Clears data generated in create_chbtex func if necessary.
	void clear_gendata();
	// Returns canonical checkerboard pattern (depth/stencil 32 bit texture).
	int* getCheckerboardPattern(const int width, const int height);
	// Makes all buffers and tests to get active for next rendering in texture pass.
	// Turning Stencil test on current checkerboard framebuffer to state based on chb_active value.
	void prerender_call();
	// Makes all buffers and tests to get active for next rendering in backbuffer from texture data.
	// Disables Depth and Stencil tests, resets Clear color to white, sets Polygon mode to (GL_FRONT_AND_BACK, GL_FILL).
	void postrender_call();
	// Clears all data created for CHB_FBO assistance.
	void terminate();
	// Resizes checkerboard pattern and framebuffer to given size. May be memory leak?
	void resize(int width, int height);
	// Restores checkerboard pattern in stencil buffer to canonical value. 
	// Use only if you changed values in Stencil buffer in first pass after prerencer_call.
	void restore_checkerboard();
	void toggle_checkerboard();
	chb_fbo(const int width, const int height, bool checkerboard_mode = true, int interpolation = 0, int quad_VAO = -1, bool filtermode_linear = true);
	~chb_fbo();
};

