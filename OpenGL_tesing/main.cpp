#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "shader.cpp"
#include "configuration.h"
#include "ui_module.h"
#include "frames_counter.h"
#include "game_object.h";
#include "chb_fbo.h";

configuration config;
ui_module ui("fonts/arial.ttf");
frames_counter fps;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool isCarcasMode = false;
bool isFpsCounter = false;
bool isCheckerboardRendering = true;
const float NEAR_CLIPPING_PLANE_DIST = 0.1f;
const float FAR_CLIPPING_PLANE_DIST = 100.0f;
const float FOV = 45.0f;
const glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
const glm::vec3 lightInitPos(2.0f, 1.0f, 5.0f);
const glm::vec3 globalLightColor(1.0f, 1.0f, 1.0f);
int cube_rotation_speed = 1;
bool msaa_enabled = false;

float deltaTime = 0.0f;

int configuration::WINDOW_WIDTH = 800;
int configuration::WINDOW_HEIGHT = 600;
float configuration::NEAR_CLIPPING_PLANE_DIST = 0.1f;
float configuration::FAR_CLIPPING_PLANE_DIST = 100.0f;
float configuration::FOV = 45.0f;
float configuration::CAMERA_OFFSET = 6.5f;

chb_fbo* framebuffer = nullptr;

int cubeCount = 43;
glm::vec3 cubePositions[] =
{
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-2.0f, 0.0f, 0.0f),
	glm::vec3(-2.0f, 2.0f, 0.0f),
	glm::vec3(0.0f, 2.0f, 0.0f),
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, -2.0f, 0.0f),
	glm::vec3(0.0f, -2.0f, 0.0f),
	glm::vec3(-2.0f, -2.0f, 0.0f),
	glm::vec3(-4.0f, -2.0f, 0.0f),
	glm::vec3(-4.0f, 0.0f, 0.0f),
	glm::vec3(-4.0f, 2.0f, 0.0f),
	glm::vec3(4.0f, -2.0f, 0.0f),
	glm::vec3(4.0f, 0.0f, 0.0f),
	glm::vec3(4.0f, 2.0f, 0.0f),
	glm::vec3(3.0f, 1.0f, 0.0f),
	glm::vec3(3.0f, -1.0f, 0.0f),
	glm::vec3(-3.0f, 1.0f, 0.0f),
	glm::vec3(-3.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),

	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(-3.0f, 0.0f, -1.0f),
	glm::vec3(-3.0f, 2.0f, -1.0f),
	glm::vec3(-1.0f, 2.0f, -1.0f),
	glm::vec3(1.0f, 2.0f, -1.0f),
	glm::vec3(1.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, -2.0f, -1.0f),
	glm::vec3(-1.0f, -2.0f, -1.0f),
	glm::vec3(-3.0f, -2.0f, -1.0f),
	glm::vec3(3.0f, -2.0f, -1.0f),
	glm::vec3(3.0f, 0.0f, -1.0f),
	glm::vec3(3.0f, 2.0f, -1.0f),
	glm::vec3(2.0f, 1.0f, -1.0f),
	glm::vec3(2.0f, -1.0f, -1.0f),
	glm::vec3(-4.0f, 1.0f, -1.0f),
	glm::vec3(-4.0f, -1.0f, -1.0f),
	glm::vec3(-2.0f, -1.0f, -1.0f),
	glm::vec3(-2.0f, 1.0f, -1.0f),
	glm::vec3(0.0f, -1.0f, -1.0f),
	glm::vec3(0.0f, 1.0f, -1.0f),
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	configuration::WINDOW_WIDTH = width;
	configuration::WINDOW_HEIGHT = height;
	/*glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);*/
	/*glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);*/
	glViewport(0, 0, width, height);
	framebuffer->resize(width, height);
	std::cout << "---RESIZED---" << std::endl;
}

void config_init() 
{
	config.init();
	WINDOW_WIDTH = config["window_width"];
	WINDOW_HEIGHT = config["window_height"];
	isCarcasMode = config["carcas_mode"];
	isFpsCounter = config["fps_counter"];
	isCheckerboardRendering = config["checkerboard_r"];
	cube_rotation_speed = config["cube_rotation_speed"];
	cubeCount = config["cube_count"];
	msaa_enabled = config["msaa"];
}

void getError() 
{
	auto glstatus = glGetError();
	if (glstatus != GL_NO_ERROR)
		std::cout << "Error in GL call:" << glstatus << std::endl;
}

int main() 
{
	config_init();

	// Инициализация компонентов
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (msaa_enabled)
		glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	ui.set_font_size(48);
	ui.load_characters();
	
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Создание матрицы ортографической проекции
	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
	// Создание матрицы перспективной проекции проекции
	/*glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
		NEAR_CLIPPING_PLANE_DIST, FAR_CLIPPING_PLANE_DIST);*/

	// SHADERS
	glm::vec3 yellow(1.0f, 1.0f, 0.0f);
	glm::vec3 lightPos(lightInitPos);
	shader textShader("shaders/text_vertex_shader.glsl", "shaders/text_fragment_shader.glsl");
	textShader.use();
	// must do it before resize
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
	shader solidColorOrange("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader_orange.glsl");
	shader solidColorYellow("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader_yellow.glsl");
	shader lightingShader("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader.glsl");
	shader texturedShader("shaders/simple_vertex_shader.glsl", "shaders/textured_fragment_shader.glsl");
	texturedShader.use();
	texturedShader.setInt("material.diffuse", 0);

	framebuffer = new chb_fbo(WINDOW_WIDTH, WINDOW_HEIGHT, isCheckerboardRendering);
	framebuffer->clear_gendata();

	// Cube vertices
	float cube[] = {
		// координаты        // нормали           // текстурные координаты
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// MESH
	mesh cubeMesh;
	cubeMesh.vertices = cube;
	cubeMesh.v_count = 36;
	// MATERIAL
	material cubeMaterial;
	cubeMaterial.path_to_texture = "materials/cubeTexture.jpg";
	cubeMaterial.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeMaterial.shininess = 32.0f;
	// GLOBAL LIGHT
	light globalLight;
	globalLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	globalLight.diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	globalLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	globalLight.position = lightPos;
	//transform cubePos;
	//cubePos.world_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	game_object cube_object(&cubeMesh, &texturedShader);
	cube_object.set_material(&cubeMaterial);
	cube_object.set_global_light(&globalLight);
	cube_object.gen_buffers();

	// BACKGROUND
	glClearColor(0.14901f, 0.16862f, 0.17647f, 1.0f);
	if (msaa_enabled)
		glEnable(GL_MULTISAMPLE);

	float lastFrame = 0.0f;

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window)) 
	{
		// Пользовательский ввод
		processInput(window);

		// CARCAS MODE
		if (isCarcasMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// drawing | Первый проход
		framebuffer->use();
		glEnable(GL_DEPTH_TEST);
		framebuffer->prerender_call();
		glClearColor(0.14901f, 0.16862f, 0.17647f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * cube_rotation_speed, glm::vec3(0.5f, 1.0f, 0.0f));
		for (int i = 0; i < cubeCount; ++i) 
		{
			cube_object.set_position(cubePositions[i], model);
			cube_object.render();
		}

		// UI SECTION
		if (isFpsCounter)
			ui.render_text(textShader, std::to_string(fps.get_fps()), 25.0f, /*25.0f*/ WINDOW_HEIGHT - ui.get_font_size(), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// drawing | Второй проход (в screen quad)
		framebuffer->postrender_call();
			
		// Проверка и обработка событий, обмен содержимого буферов
		glfwSwapInterval(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

		// FRAME TIME
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		fps.add_frame(deltaTime);
		lastFrame = currentFrame;
	}

	framebuffer->terminate();
	delete framebuffer;
	glfwTerminate();
	fps.log_in_file();
	return 0;
}