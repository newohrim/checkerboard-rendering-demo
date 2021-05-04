#include <glad/glad.h>
#include <iostream>
#include "shader.cpp"

#pragma once
class chb_fbo
{
private:
	const unsigned int DEFFAULT_FRAMEBUFFER = 0;
	const char* PATH_TO_VERTEXSHADER = "shaders/fbo_to_screenquad_vertex_shader.glsl";
	const char* PATH_TO_SIMPLEFRAGMENTSHADER = "shaders/fbo_to_screenquad_simple_shader.glsl";
	const char* PATH_TO_CHBFRAGMENTSHADER = "shaders/fbo_to_screenquad_fragment_shader.glsl";
	unsigned int fbo_id;
	int fbo_width;
	int fbo_height;
	int* checkerboard;
	unsigned int checkerboard_tex;
	unsigned int screen_texeven;
	unsigned int screen_texodd;
	unsigned int screen_quadVBO;
	unsigned int screen_quadVAO;
	const bool filter_linear = true;
	bool is_oddframe = false;
	const bool chb_active = true;
	shader* simple_screenquadshader;
	shader* chb_screenquadshader;
	void create_fbo();
	void create_chbtex();
	void create_screentex();
	void create_screenquad();
	void set_attachments();
	void fbo_checkerror();
	void getError();

	float quad_vertices[24] = { 
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
public:
	unsigned int get_id();
	void use();
	void clear_gendata();
	int* getCheckerboardPattern(const int width, const int height);
	void prerender_call();
	void postrender_call();
	void terminate();
	void resize(int width, int height);
	chb_fbo(const int width, const int height, bool checkerboard_mode = true, int quad_VAO = -1, bool filtermode_linear = true);
	~chb_fbo();
};

