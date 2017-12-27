//#pragma once
//// C++标准库
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <vector>
//using namespace std;
//// GL库
//#include <GL/glew.h> 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "Shader.h"
//#include <assimp\types.h>
//
//struct Vertex {
//	// 顶点位置坐标
//	glm::vec3 Position;
//	// 顶点法线坐标
//	glm::vec3 Normal;
//	// 顶点纹理坐标
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
//	/*  网格数据  */
//	vector<Vertex> vertices;
//	vector<GLuint> indices;
//	vector<Texture> textures;
//
//	/*  功能  */
//	// 构造函数
//	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
//	{
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//
//		// 设置顶点缓冲和属性指针
//		this->setupMesh();
//	}
//
//	// 渲染网格
//	void Draw(Shader shader)
//	{
//		// 绑定适当的纹理
//		GLuint diffuseNr = 1;
//		GLuint specularNr = 1;
//		for (GLuint i = 0; i < this->textures.size(); i++)
//		{
//
//			glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活适当的纹理单元
//											  // 检索纹理数字 (the N in diffuse_textureN)
//			stringstream ss;
//			string number;
//			string name = this->textures[i].type;
//			if (name == "texture_diffuse")
//				ss << diffuseNr++; // 把Gluint传输到流
//			else if (name == "texture_specular")
//				ss << specularNr++; // 把Gluint传输到流
//			number = ss.str();
//			//std::cout << name + number << std::endl;
//			// 给采样器设置正确的纹理单元
//			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
//			// 最终绑定纹理
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
//		// 渲染网格
//		glBindVertexArray(this->VAO);
//		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// 解绑
//		for (GLuint i = 0; i < this->textures.size(); i++)
//		{
//			glActiveTexture(GL_TEXTURE0 + i);
//			glBindTexture(GL_TEXTURE_2D, 0);
//		}
//	}
//
//private:
//	/*  渲染数据  */
//	GLuint VAO, VBO, EBO;
//
//	/*  功能    */
//	// 初始化所有的缓冲对象和数组
//	void setupMesh()
//	{
//		// 创造VAO、VBO
//		glGenVertexArrays(1, &this->VAO);
//		glGenBuffers(1, &this->VBO);
//		glGenBuffers(1, &this->EBO);
//
//		glBindVertexArray(this->VAO);
//		// 加载数据进顶点缓冲
//		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//
//		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
//
//		// 设置顶点属性指针
//		// 顶点位置
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//		// 法线
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
//		// 纹理坐标
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
// C++标准库
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL库
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <assimp\types.h>

struct Vertex {
	// 顶点位置坐标
	glm::vec3 Position;
	// 顶点法线坐标
	glm::vec3 Normal;
	// 顶点纹理坐标
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	/*  网格数据  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	/*  功能  */
	// 构造函数
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// 设置顶点缓冲和属性指针
		this->setupMesh();
	}

	// 渲染网格
	void Draw(Shader shader)
	{
		// 绑定适当的纹理
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{

			glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活适当的纹理单元
											  // 检索纹理数字 (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // 把Gluint传输到流
			else if (name == "texture_specular")
				ss << specularNr++; // 把Gluint传输到流
			number = ss.str();
			//std::cout << name + number << std::endl;
			// 给采样器设置正确的纹理单元
			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			// 最终绑定纹理
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);

			/*if (diffuseNr > 1)
			std::cout << "diffuseNr = " << diffuseNr << std::endl;
			if (specularNr > 1)
			std::cout << "specularNr = " << specularNr << std::endl;*/

		}



		// 渲染网格
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// 解绑
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	/*  渲染数据  */
	GLuint VAO, VBO, EBO;

	/*  功能    */
	// 初始化所有的缓冲对象和数组
	void setupMesh()
	{
		// 创造VAO、VBO
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// 加载数据进顶点缓冲
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// 设置顶点属性指针
		// 顶点位置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// 法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// 纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};



