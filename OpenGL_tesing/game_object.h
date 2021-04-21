#include "shader.cpp"
#include "stb_image.h"
#include "configuration.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>

struct transform 
{
	glm::vec3 world_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection;
};

struct material 
{
	const char* path_to_texture;
	unsigned int texture_id = 0;
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	float shininess = 0.0f;
};

struct mesh 
{
	unsigned int VBO;
	unsigned int VAO;
	int v_count;
	float* vertices;
};

struct light 
{
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

#pragma once
class game_object
{
private:
	mesh obj_mesh;
	material* obj_material;
	transform obj_position;
	shader* obj_shader;
	light* global_light;

	glm::mat4 get_perspective();
public:
	void render();
	void gen_buffers();
	void set_material(material* mat);
	void set_position(glm::vec3 pos, glm::mat4 model);
	void set_global_light(light* gl_light);
	game_object(mesh* mesh, shader* shader) 
	{
		obj_mesh.v_count = mesh->v_count;
		obj_mesh.vertices = mesh->vertices;
		obj_shader = shader;
	}
	~game_object();
};

