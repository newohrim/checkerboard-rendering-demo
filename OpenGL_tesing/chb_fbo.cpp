#include "chb_fbo.h"

void chb_fbo::create_fbo()
{
	// Creating one framebuffer.
	glGenFramebuffers(1, &fbo_id);
}

void chb_fbo::create_chbtex()
{
	// Generating data for depth/stencil texture.
	checkerboard = getCheckerboardPattern(fbo_width, fbo_height);
	glGenTextures(1, &checkerboard_tex);
	glBindTexture(GL_TEXTURE_2D, checkerboard_tex);
	// Setting options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// not sure about that
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
	// Setting texture inner formats and attaching generated data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, fbo_width, fbo_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, checkerboard);
	getError();
	glBindTexture(GL_TEXTURE_2D, 0);
	//delete[] data;
}

void chb_fbo::create_screentex()
{
	// Create even screen texture.
	glGenTextures(1, &screen_texeven);
	glBindTexture(GL_TEXTURE_2D, screen_texeven);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_width, fbo_height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + filter_linear);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + filter_linear);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create odd screen texture.
	glGenTextures(1, &screen_texodd);
	glBindTexture(GL_TEXTURE_2D, screen_texodd);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_width, fbo_height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + filter_linear);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + filter_linear);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void chb_fbo::create_screenquad()
{
	// Create screen quad if necessary.
	glGenBuffers(1, &screen_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screen_quadVBO);
	glGenVertexArrays(1, &screen_quadVAO);
	glBindVertexArray(screen_quadVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	// VERTICES
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// TEXTURE COORDS
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void chb_fbo::set_attachments()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	// Setting color attachment to even screen texture.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texeven, 0);
	// Setting depth/stencil attachment to checkerboard texture.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, checkerboard_tex, 0);
}

void chb_fbo::fbo_checkerror()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::CHB_FRAMEBUFFER:: Framebuffer is not complete/supported :: " << fboStatus << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, DEFFAULT_FRAMEBUFFER);
}

unsigned int chb_fbo::get_id()
{
	return fbo_id;
}

void chb_fbo::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
}

void chb_fbo::clear_gendata()
{
	if (checkerboard != NULL)
		delete[] checkerboard;
	checkerboard = nullptr;
}

void chb_fbo::getError()
{
	auto glstatus = glGetError();
	if (glstatus != GL_NO_ERROR)
		std::cout << "Error in GL call:" << glstatus << std::endl;
}

int* chb_fbo::getCheckerboardPattern(const int width, const int height)
{
	// Array size.
	int n = height * width; 
	// 32 bit each.
	int* data = new int[n];
	// Depth/stencil fill value [depth = 0, stencil = 255].
	const int fill = 0xFF;
	int a = fill;
	bool width_odd = width & 1;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			data[i * width + j] = a;
			// inverting stencil component.
			a ^= fill;
		}
		if (!width_odd)
			a ^= fill;
	}
	return data;
}

void chb_fbo::prerender_call()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	if (chb_active)
	{
		// Enabling stecnil test.
		glEnable(GL_STENCIL_TEST);
		// Setting test func depending on is_odd value (better then inverting pattern).
		glStencilFunc(GL_EQUAL, 255 * is_oddframe, 255);
		// Diffrent color attachments depending on is_odd value.
		if (!is_oddframe)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texeven, 0);
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texodd, 0);
		}
	}
}

void chb_fbo::postrender_call()
{
	glBindFramebuffer(GL_FRAMEBUFFER, DEFFAULT_FRAMEBUFFER);
	// No need in tests when rendering a simple full screen quad.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	// Resetting clear color (helpful for debug).
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (chb_active)
	{
		chb_screenquadshader->use();
		// When even frame [id:0 = even_texture, id:1 = odd_texture].
		// When odd frame [id:0 = odd_texture, id:1 = even_texture].
		glActiveTexture(GL_TEXTURE0 + is_oddframe);
		glBindTexture(GL_TEXTURE_2D, screen_texeven);
		glActiveTexture(GL_TEXTURE0 + !is_oddframe);
		glBindTexture(GL_TEXTURE_2D, screen_texodd);
		// Naming mismath :(
		chb_screenquadshader->setBool("isEvenFrame", is_oddframe);
	}
	else
	{
		// Simple method when checkerboard disactive.
		simple_screenquadshader->use();
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, screen_texeven);
	}

	glBindVertexArray(screen_quadVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// RENDER TO BACKBUFFER
	glDrawArrays(GL_TRIANGLES, 0, 6);
	is_oddframe = !is_oddframe;
}

void chb_fbo::terminate()
{
	// call me DELETO
	glDeleteBuffers(1, &screen_quadVBO);
	glDeleteVertexArrays(1, &screen_quadVAO);
	glDeleteTextures(1, &screen_texeven);
	glDeleteTextures(1, &screen_texodd);
	glDeleteTextures(1, &checkerboard_tex);
	glDeleteFramebuffers(1, &fbo_id);
	simple_screenquadshader->terminate();
	chb_screenquadshader->terminate();
	clear_gendata();
}

void chb_fbo::resize(int width, int height)
{
	// Deleting old textures from videomemory.
	glDeleteTextures(1, &screen_texeven);
	glDeleteTextures(1, &screen_texodd);
	glDeleteTextures(1, &checkerboard_tex);
	use();
	// Clearing old depth/stencil values.
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	fbo_width = width;
	fbo_height = height;
	// Optionaly clearing checkerboard source data.
	clear_gendata();
	// Creating checkerboard depth/stencil texture again.
	create_chbtex();
	// Creating screen textures (even/odd) again.
	create_screentex();
	// Setting attachments.
	set_attachments();
	// Checking for errors.
	fbo_checkerror();
	// Resetting is_odd flag.
	is_oddframe = false;
}

void chb_fbo::restore_checkerboard()
{
	use();
	// Clearing old depth/stencil values.
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Deleteing old depth/stencil texture.
	glDeleteTextures(1, &checkerboard_tex);
	// Optionaly clearing checkerboard source data.
	clear_gendata();
	// Setting attachments again.
	set_attachments();
	// Resetting is_odd flag??? why?
	//is_oddframe = false;
}

chb_fbo::chb_fbo(const int width, const int height, bool checkerboard_mode, int interpolation, int quad_VAO, bool filtermode_linear) :
	fbo_width(width), fbo_height(height), chb_active(checkerboard_mode), interpolation_count(interpolation), filter_linear(filtermode_linear)
{
	// Creating framebuffer.
	create_fbo();
	// Creating depth/stencil texture.
	create_chbtex();
	// Creating screen textures (even/odd).
	create_screentex();
	// Setting attachments to framebuffer.
	set_attachments();
	// Checking framebuffer completeness.
	fbo_checkerror();
	glBindFramebuffer(GL_FRAMEBUFFER, DEFFAULT_FRAMEBUFFER);

	screen_quadVAO = quad_VAO;
	if (quad_VAO == -1)
		create_screenquad();

	// Getting and setting shaders.
	simple_screenquadshader = new shader(PATH_TO_VERTEXSHADER, PATH_TO_SIMPLEFRAGMENTSHADER);
	if (interpolation_count < 2) 
	{
		chb_screenquadshader = new shader(PATH_TO_VERTEXSHADER, PATH_TO_CHBFRAGMENTSHADER);
	}
	else 
	{
		chb_screenquadshader = new shader(PATH_TO_VERTEXSHADER, PATH_TO_CHBFRAGMENTSHADERINTERPOLATED);
		chb_screenquadshader->use();
		chb_screenquadshader->setInt("interp_count", interpolation_count);
	}
	chb_screenquadshader->use();
	chb_screenquadshader->setInt("screenTexture", 0);
	chb_screenquadshader->setInt("prevScreenTexture", 1);
}

chb_fbo::~chb_fbo()
{
	//glDeleteBuffers(1, &screen_quadVBO);
	//glDeleteVertexArrays(1, &screen_quadVAO);
	if (checkerboard != NULL)
		delete[] checkerboard;
	delete simple_screenquadshader;
	delete chb_screenquadshader;
}
