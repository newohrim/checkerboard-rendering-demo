#include "shader.cpp"
#include "stb_image.h"
#include "configuration.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>

// Transform object's component
struct transform 
{
	// Position of object in world coords.
	glm::vec3 world_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	// Model matrix.
	glm::mat4 model = glm::mat4(1.0f);
	// Projection matrix.
	glm::mat4 projection;
};

// Material object's component.
struct material 
{
	// Path to main diffuse texture image.
	const char* path_to_texture;
	// Diffuse texture's id.
	unsigned int texture_id = 0;
	// Specular component.
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	// Shininess component.
	float shininess = 0.0f;
};

// Mesh object's component.
struct mesh 
{
	// Vertex Buffer Object of Game object.
	unsigned int VBO;
	// Vertex Array Object of Game Object.
	unsigned int VAO;
	// Count of vertices in mesh.
	int v_count;
	// Array of vertices in mesh.
	float* vertices;
};

// Light instance on scene.
struct light 
{
	// Ambient component.
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	// Diffuse component.
	glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	// Specular component.
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	// Light's postion in world coords.
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

#pragma once
class game_object
{
private:
	// Mesh component.
	mesh obj_mesh;
	// Material component.
	material* obj_material;
	// Transform component.
	transform obj_position;
	// Object shader.
	shader* obj_shader;
	// Point to light source.
	light* global_light;

	glm::mat4 get_perspective();
public:
	// Renders object on scene.
	void render();
	// Generates buffers for object.
	void gen_buffers();
	// Sets material to object.
	void set_material(material* mat);
	// Sets world position and model matrix to object.
	void set_position(glm::vec3 pos, glm::mat4 model);
	// Sets light sourse pointer to object.
	void set_global_light(light* gl_light);
	// Initializes object with start values.
	game_object(mesh* mesh, shader* shader) 
	{
		obj_mesh.v_count = mesh->v_count;
		obj_mesh.vertices = mesh->vertices;
		obj_shader = shader;
	}
	~game_object();
};

