#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL\glew.h>

class Shader {
public:
	// ����ID
	GLuint Program;
	// ��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ�ó���
	void Use();
};

#endif // !SHADER_H
