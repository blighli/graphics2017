#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL\glew.h>

class Shader {
public:
	// 程序ID
	GLuint Program;
	// 构造器读取并构建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用程序
	void Use();
};

#endif // !SHADER_H
