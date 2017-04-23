//#pragma once
//// C++��׼��
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <vector>
//using namespace std;
//// GL��
//#include <GL/glew.h> 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "Shader.h"
//#include <assimp\types.h>
//
//struct Vertex {
//	// ����λ������
//	glm::vec3 Position;
//	// ���㷨������
//	glm::vec3 Normal;
//	// ������������
//	glm::vec2 TexCoords;
//};
//
//struct Texture {
//	GLuint id;
//	string type;
//	aiString path;
//};
//
//class Mesh {
//public:
//	/*  ��������  */
//	vector<Vertex> vertices;
//	vector<GLuint> indices;
//	vector<Texture> textures;
//
//	/*  ����  */
//	// ���캯��
//	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
//	{
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//
//		// ���ö��㻺�������ָ��
//		this->setupMesh();
//	}
//
//	// ��Ⱦ����
//	void Draw(Shader shader)
//	{
//		// ���ʵ�������
//		GLuint diffuseNr = 1;
//		GLuint specularNr = 1;
//		for (GLuint i = 0; i < this->textures.size(); i++)
//		{
//
//			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ�����ʵ�������Ԫ
//											  // ������������ (the N in diffuse_textureN)
//			stringstream ss;
//			string number;
//			string name = this->textures[i].type;
//			if (name == "texture_diffuse")
//				ss << diffuseNr++; // ��Gluint���䵽��
//			else if (name == "texture_specular")
//				ss << specularNr++; // ��Gluint���䵽��
//			number = ss.str();
//			//std::cout << name + number << std::endl;
//			// ��������������ȷ������Ԫ
//			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
//			// ���հ�����
//			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
//
//			/*if (diffuseNr > 1)
//			std::cout << "diffuseNr = " << diffuseNr << std::endl;
//			if (specularNr > 1)
//			std::cout << "specularNr = " << specularNr << std::endl;
//*/
//		}
//
//
//
//		// ��Ⱦ����
//		glBindVertexArray(this->VAO);
//		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// ���
//		for (GLuint i = 0; i < this->textures.size(); i++)
//		{
//			glActiveTexture(GL_TEXTURE0 + i);
//			glBindTexture(GL_TEXTURE_2D, 0);
//		}
//	}
//
//private:
//	/*  ��Ⱦ����  */
//	GLuint VAO, VBO, EBO;
//
//	/*  ����    */
//	// ��ʼ�����еĻ�����������
//	void setupMesh()
//	{
//		// ����VAO��VBO
//		glGenVertexArrays(1, &this->VAO);
//		glGenBuffers(1, &this->VBO);
//		glGenBuffers(1, &this->EBO);
//
//		glBindVertexArray(this->VAO);
//		// �������ݽ����㻺��
//		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//
//		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
//
//		// ���ö�������ָ��
//		// ����λ��
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//		// ����
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
//		// ��������
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
//
//		glBindVertexArray(0);
//	}
//};
//
//
//
#pragma once
// C++��׼��
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL��
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <assimp\types.h>

struct Vertex {
	// ����λ������
	glm::vec3 Position;
	// ���㷨������
	glm::vec3 Normal;
	// ������������
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	/*  ��������  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	/*  ����  */
	// ���캯��
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// ���ö��㻺�������ָ��
		this->setupMesh();
	}

	// ��Ⱦ����
	void Draw(Shader shader)
	{
		// ���ʵ�������
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{

			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ�����ʵ�������Ԫ
											  // ������������ (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // ��Gluint���䵽��
			else if (name == "texture_specular")
				ss << specularNr++; // ��Gluint���䵽��
			number = ss.str();
			//std::cout << name + number << std::endl;
			// ��������������ȷ������Ԫ
			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			// ���հ�����
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);

			/*if (diffuseNr > 1)
			std::cout << "diffuseNr = " << diffuseNr << std::endl;
			if (specularNr > 1)
			std::cout << "specularNr = " << specularNr << std::endl;*/

		}



		// ��Ⱦ����
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// ���
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	/*  ��Ⱦ����  */
	GLuint VAO, VBO, EBO;

	/*  ����    */
	// ��ʼ�����еĻ�����������
	void setupMesh()
	{
		// ����VAO��VBO
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// �������ݽ����㻺��
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// ���ö�������ָ��
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// ����
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// ��������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};



