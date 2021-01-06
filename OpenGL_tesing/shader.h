//#ifndef SHADER_H
//#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class shader
{
public:
    // ID - идентификатор программы
    unsigned int ID;

    // Конструктор считывает данные и выполняет построение шейдера
    shader(const char* vertexPath, const char* fragmentPath);

    // Использование/активация шейдера
    void use();

    // Полезные uniform-функции
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

//#endif