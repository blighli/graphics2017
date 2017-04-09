#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ��֤ifstream��������׳��쳣
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ��嵽����
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();
		// ת������GLchar����
		vertexCode = vShaderStream.str();
		//std::cout << vertexCode << std::endl;

		fragmentCode = fShaderStream.str();
		//std::cout << fragmentCode << std::endl;
	} catch (std::ifstream::failure e) {
		std::cout << "Error::Shader::File_Not_Successfully_Read" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// ������ɫ��
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// ������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Error::Shader::Vertex::Compilation_Failed\n" << infoLog << std::endl;
	}

	// Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Error::Shader::Fragment::Compilation_Failed\n" << infoLog << std::endl;
	}

	// ��ɫ������
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		std::cout << "Error::Shader::Program::Linking_Failed\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() {
	glUseProgram(Program);
}