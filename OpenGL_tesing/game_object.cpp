#include "game_object.h"

void game_object::gen_buffers()
{
	glGenBuffers(1, &obj_mesh.VBO);
	glGenVertexArrays(1, &obj_mesh.VAO);
	glBindVertexArray(obj_mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj_mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, obj_mesh.v_count * 8 * sizeof(float), obj_mesh.vertices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Атрибуты нормалей
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Атрибуты текстуры
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Загрузка текстуры
	int texture_width, texture_height, nr_channels;
	unsigned char* textureData = stbi_load(obj_material->path_to_texture,
		&texture_width, &texture_height, &nr_channels, 0);

	// Генерация текстуры
	glGenTextures(1, &obj_material->texture_id);
	glBindTexture(GL_TEXTURE_2D, obj_material->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 
			0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(textureData);
}

glm::mat4 game_object::get_perspective()
{
	return glm::perspective(glm::radians(configuration::FOV), 
		(float)configuration::WINDOW_WIDTH / (float)configuration::WINDOW_HEIGHT,
		configuration::NEAR_CLIPPING_PLANE_DIST, configuration::FAR_CLIPPING_PLANE_DIST);
}

void game_object::render()
{
	obj_shader->use();
	obj_shader->setVec3("viewPos", 0.0f, 0.0f, configuration::CAMERA_OFFSET);
	obj_shader->setVec3("material.specular", 
		obj_material->specular.x, obj_material->specular.y, obj_material->specular.z);
	obj_shader->setFloat("material.shininess", obj_material->shininess);
	obj_shader->setVec3("light.ambient", 
		global_light->ambient.x, global_light->ambient.y, global_light->ambient.z);
	obj_shader->setVec3("light.diffuse", 
		global_light->diffuse.x, global_light->diffuse.y, global_light->diffuse.z);
	obj_shader->setVec3("light.specular", 
		global_light->specular.x, global_light->specular.y, global_light->specular.z);
	obj_shader->setVec3("light.position", 
		global_light->position.x, global_light->position.y, global_light->position.z);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(obj_position.world_pos.x, obj_position.world_pos.y, 
		obj_position.world_pos.z - configuration::CAMERA_OFFSET));
	int modelLoc = glGetUniformLocation(obj_shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(obj_position.model));
	int viewLoc = glGetUniformLocation(obj_shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	int projLoc = glGetUniformLocation(obj_shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(get_perspective()));
	glBindVertexArray(obj_mesh.VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj_material->texture_id);
	glDrawArrays(GL_TRIANGLES, 0, obj_mesh.v_count);
}

void game_object::set_material(material* mat)
{
	obj_material = mat;
}

void game_object::set_position(glm::vec3 pos, glm::mat4 model)
{
	obj_position.world_pos = pos;
	obj_position.model = model;
}

void game_object::set_global_light(light* gl_light)
{
	global_light = gl_light;
}

game_object::~game_object()
{
	glDeleteBuffers(1, &obj_mesh.VBO);
	glDeleteVertexArrays(1, &obj_mesh.VAO);
}
