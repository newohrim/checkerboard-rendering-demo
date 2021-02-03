#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "shader.cpp"
#include "stb_image.h"
#include "configuration.h"

configuration config;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool isCarcasMode = false;
const float NEAR_CLIPPING_PLANE_DIST = 0.1f;
const float FAR_CLIPPING_PLANE_DIST = 100.0f;
const float FOV = 45.0f;

unsigned int texColorBuffer = 0;
unsigned int prevTexColorBuffer = 0;
unsigned int rbo = 0;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);*/
	glViewport(0, 0, width, height);
	std::cout << "---RESIZED---" << std::endl;
}

glm::mat4 getPerspective() 
{
	return glm::perspective(glm::radians(FOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
		NEAR_CLIPPING_PLANE_DIST, FAR_CLIPPING_PLANE_DIST);
}

unsigned char* getCheckerboardPattern(const int width, const int height) 
{
	int n = height * width; // 480 000 symbols
	unsigned char* data = new unsigned char[n + 1];
	if (data == NULL)
		std::cout << "WTF??? LOL." << std::endl;
	const unsigned char fill = 0xFF;
	unsigned char a = fill;
	for (int i = 0; i < height; ++i) 
	{
		for (int j = 0; j < width; ++j) 
		{
			data[i * width + j] = a;
			//std::cout << "(" << i << "; " << j << ") : " << (int)data[i * width + j] << std::endl;
			a ^= fill;
		}
		a ^= fill;
	}
	data[n] = '\0';
	/*for (int i = 0; i < n; ++i) 
		if (data[i] != 0 && data[i] != 255)
			std::cout << "WTF??? LOL.2" << std::endl;*/
	return data;
}

void config_init() 
{
	config.init();
	WINDOW_WIDTH = config["window_width"];
	WINDOW_HEIGHT = config["window_height"];
	isCarcasMode = config["carcas_mode"];
}

int main() 
{
	config_init();

	// Инициализация компонентов
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Создание матрицы ортографической проекции
	glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, NEAR_CLIPPING_PLANE_DIST, FAR_CLIPPING_PLANE_DIST);
	// Создание матрицы перспективной проекции проекции
	/*glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
		NEAR_CLIPPING_PLANE_DIST, FAR_CLIPPING_PLANE_DIST);*/

	// SHADERS
	shader solidColorOrange("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader_orange.glsl");
	shader solidColorYellow("shaders/simple_vertex_shader.glsl", "shaders/solid_color_fragment_shader_yellow.glsl");
	shader screenQuadShader("shaders/fbo_to_screenquad_vertex_shader.glsl", "shaders/fbo_to_screenquad_fragment_shader.glsl");
	screenQuadShader.use();
	screenQuadShader.setInt("screenTexture", 0);
	screenQuadShader.setInt("prevScreenTexture", 1);

	// TEXTURES
	//int width, height, nrChannels;
	unsigned char* data = getCheckerboardPattern(WINDOW_WIDTH, WINDOW_HEIGHT);//= stbi_load("materials/ColoredCheckerboard800x600.png", &width, &height, &nrChannels, 0);

	/*unsigned int checkerboardPattern;
	glGenTextures(1, &checkerboardPattern);
	glBindTexture(GL_TEXTURE_2D, checkerboardPattern);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);*/

	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);*/

	//data = stbi_load("materials/checkerboard800x600_alpha.png", &width, &height, &nrChannels, 0);

	unsigned int checkerboardPatternAplha;
	glGenTextures(1, &checkerboardPatternAplha);
	glBindTexture(GL_TEXTURE_2D, checkerboardPatternAplha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] data;
	//stbi_image_free(data);

	// STENCIL BUFFER
	/*unsigned int sb;
	glGenRenderbuffers(2, &sb);
	glBindRenderbuffer(GL_RENDERBUFFER, sb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);*/

	// Вершины
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // левая вершина
		 0.5f, -0.5f, 0.0f, // правая вершина
		 0.0f,  0.5f, 0.0f  // верхняя вершина 
	};

	float vertices2[] = {
		 1.0f,  0.5f, 0.0f, // правая вершина
		 0.5f, -0.5f, 0.0f, // нижняя вершина
		 0.0f,  0.5f, 0.0f  // левая вершина 
	};

	float cube[] = {
		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  

		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  

		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  
	};
	unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};

	float quadVertices[] = { // атрибуты вершин в нормализованных координатах устройства для прямоугольника, который имеет размеры экрана 
		 // координаты // текстурные координаты
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// VBO, VAO, EBO
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// VAO прямоугольника
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// FBO
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Генерируем текстуру
	//unsigned int texColorBuffer; // объявляется выше
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Прикрепляем её к текущему связанному объекту фреймбуфера
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, checkerboardPatternAplha, 0);

	// RBO
	//unsigned int rbo; // объявляется выше
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Генерируем текстуру для реконстуркции кадра
	//unsigned int texColorBuffer; // объявляется выше
	glGenTextures(1, &prevTexColorBuffer);
	glBindTexture(GL_TEXTURE_2D, prevTexColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// BACKGROUND
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	float lastFrame = 0.0f;
	int frameNum = 0;

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window)) 
	{
		// Пользовательский ввод
		processInput(window);

		// CARCAS MODE
		if (isCarcasMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// drawing | Первый проход
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		if (frameNum == 0)
			glStencilFunc(GL_EQUAL, 0, 255);
		else
			glStencilFunc(GL_EQUAL, 255, 255);
		//glEnable(GL_COLOR_LOGIC_OP);
		//glLogicOp(GL_NOOP);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		solidColorYellow.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		int modelLoc = glGetUniformLocation(solidColorYellow.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(solidColorYellow.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(solidColorYellow.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(getPerspective()));
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// drawing | Второй проход (в screen quad)
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // назад к значениям по умолчанию
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.use();
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, prevTexColorBuffer);
		glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, prevTexColorBuffer);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			
		// Проверка и обработка событий, обмен содержимого буферов
		glfwSwapBuffers(window);
		glfwPollEvents();

		// FRAME TIME
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::cout << "frame time: " << deltaTime << std::endl;
		//std::cout << "frame num: " << frameNum << std::endl;
		frameNum = ++frameNum & 1;
		screenQuadShader.setBool("isEvenFrame", frameNum);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}