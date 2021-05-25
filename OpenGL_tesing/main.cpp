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

// CONFIG
configuration config;
// UI MODULE
ui_module ui("fonts/arial.ttf");
// FPS COUNTER
frames_counter fps;
// CHECKERBOARD FRAMEBUFFER OBJECT
chb_fbo* framebuffer = nullptr;

bool isCarcasMode = false;
bool isFpsCounter = false;
bool isCheckerboardRendering = true;
const glm::vec3 lightInitPos(2.0f, 1.0f, 5.0f);
int cube_rotation_speed = 1;
int interp_count = 1;
bool vsync = false;

// FRAME TIME
float deltaTime = 0.0f;

// INIT GLOBAL VALUES
int configuration::WINDOW_WIDTH = 800;
int configuration::WINDOW_HEIGHT = 600;
float configuration::NEAR_CLIPPING_PLANE_DIST = 0.1f;
float configuration::FAR_CLIPPING_PLANE_DIST = 100.0f;
float configuration::FOV = 45.0f;
float configuration::CAMERA_OFFSET = 6.5f;

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

// Proccedes users input.
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) 
	{
		std::cout << "checkerboard mode toggled" << endl;
		framebuffer->toggle_checkerboard();
	}
}

// Runs when window size changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width > 0 && height > 0 && 
		(width != configuration::WINDOW_WIDTH || height != configuration::WINDOW_HEIGHT)) 
	{
		configuration::WINDOW_WIDTH = width;
		configuration::WINDOW_HEIGHT = height;
		glViewport(0, 0, width, height);
		framebuffer->resize(width, height);
		std::cout << "---RESIZED---" << std::endl;
	}
}

// Inits config
void config_init() 
{
	config.init();
	isCarcasMode = config["carcas_mode"];
	isFpsCounter = config["fps_counter"];
	isCheckerboardRendering = config["checkerboard_r"];
	cube_rotation_speed = config["cube_rotation_speed"];
	cubeCount = config["cube_count"];
	interp_count = config["interpolation_count"];
	vsync = config["vsync"];
}

// Check for errors
void getError() 
{
	auto glstatus = glGetError();
	if (glstatus != GL_NO_ERROR)
		std::cout << "Error in GL call:" << glstatus << std::endl;
}

int main() 
{
	// CONFIG INIT
	config_init();

	// Initialize of GLFW components
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
	// Creating window
	GLFWwindow* window = glfwCreateWindow(configuration::WINDOW_WIDTH, configuration::WINDOW_HEIGHT, "Checkerboard Demo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	
	// Loading glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setting font size
	ui.set_font_size(48);
	// Loading characters
	ui.load_characters();
	// Setting View Port
	glViewport(0, 0, configuration::WINDOW_WIDTH, configuration::WINDOW_HEIGHT);
	// Creating matrix of orthographic projection for text
	glm::mat4 ortho = glm::ortho(0.0f, static_cast<float>(configuration::WINDOW_WIDTH), 0.0f, static_cast<float>(configuration::WINDOW_HEIGHT));

	// SHADERS
	glm::vec3 yellow(1.0f, 1.0f, 0.0f);
	glm::vec3 lightPos(lightInitPos);
	shader textShader("shaders/text_vertex_shader.glsl", "shaders/text_fragment_shader.glsl");
	textShader.use();
	// must do it before resize ??
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
	shader lightingShader("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader.glsl");
	shader texturedShader("shaders/simple_vertex_shader.glsl", "shaders/textured_fragment_shader.glsl");
	texturedShader.use();
	texturedShader.setInt("material.diffuse", 0);

	// Creating checkerboard framebuffer object
	framebuffer = new chb_fbo(configuration::WINDOW_WIDTH, configuration::WINDOW_HEIGHT, isCheckerboardRendering, interp_count);
	// Clearing data generated for stencil texture.
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

	float lastFrame = 0.0f;

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window)) 
	{
		// Users input
		processInput(window);

		// CARCAS MODE
		if (isCarcasMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// drawing | First pass
		framebuffer->use();
		glEnable(GL_DEPTH_TEST);
		framebuffer->prerender_call();
		glClearColor(0.14901f, 0.16862f, 0.17647f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rotating cubes around depending on time.
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * cube_rotation_speed, glm::vec3(0.5f, 1.0f, 0.0f));
		for (int i = 0; i < cubeCount; ++i) 
		{
			// RENDER TO TEXTURE
			cube_object.set_position(cubePositions[i], model);
			cube_object.render();
		}

		// UI SECTION
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (isFpsCounter)
			ui.render_text(textShader, std::to_string(fps.get_fps()), 25.0f, 
				configuration::WINDOW_HEIGHT - ui.get_font_size() - 0.02 * configuration::WINDOW_HEIGHT, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// drawing | Second pass
		// RENDER TO BACKBUFFER FROM TEXTURE
		framebuffer->postrender_call();
			
		// Proceeding events, swapping buffers
		glfwSwapInterval(vsync);
		glfwSwapBuffers(window);
		glfwPollEvents();

		// FRAME TIME
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		fps.add_frame(deltaTime);
		lastFrame = currentFrame;
	}

	// Termination framebuffer
	framebuffer->terminate();
	delete framebuffer;
	// Ui terminates
	ui.terminate();
	// GLFW terminates
	glfwTerminate();
	// Report calculations to file.
	fps.log_in_file();
	// Exit.
	return 0;
}