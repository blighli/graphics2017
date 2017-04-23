//#pragma once
//// C++标准库
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//using namespace std;
//// GL库
//#include <GL/glew.h> 
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <SOIL\SOIL.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include "Mesh.h"
//
//GLint TextureFromFile(const char* path, string directory);
//
//class Model
//{
//public:
//	/*  功能   */
//	// 构造函数
//	Model(GLchar* path)
//	{
//		this->loadModel(path);
//	}
//
//	// 绘制模型
//	void Draw(Shader shader)
//	{
//		for (GLuint i = 0; i < this->meshes.size(); i++)
//			this->meshes[i].Draw(shader);
//	}
//
//private:
//	/*  模型数据  */
//	vector<Mesh> meshes;
//	string directory;
//	vector<Texture> textures_loaded;	// 存储已经加载的纹理，防止重复加载
//
//										/*  功能   */
//										// 用ASSIMP扩展加载模型并且把网格结果存储到mesh vector中
//	void loadModel(string path)
//	{
//		// 用ASSIMP读取文件，并转换模型为三角形以及将图像沿y轴反转
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//		// 检查是否出错
//		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//		{
//			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
//			return;
//		}
//		// 检索文件的目录路径
//		this->directory = path.substr(0, path.find_last_of('/'));
//
//		// 递归处理场景的节点
//		this->processNode(scene->mRootNode, scene);
//	}
//
//	// 递归处理节点以及叶子节点
//	void processNode(aiNode* node, const aiScene* scene)
//	{
//		// 处理当前节点下的各个网格
//		for (GLuint i = 0; i < node->mNumMeshes; i++)
//		{
//			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//			this->meshes.push_back(this->processMesh(mesh, scene));
//		}
//		// 处理叶子节点
//		for (GLuint i = 0; i < node->mNumChildren; i++)
//		{
//			this->processNode(node->mChildren[i], scene);
//		}
//
//	}
//
//	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//	{
//		// 数据填充
//		// 顶点
//		vector<Vertex> vertices;
//		// 索引
//		vector<GLuint> indices;
//		// 纹理
//		vector<Texture> textures;
//
//		// 遍历网格顶点
//		for (GLuint i = 0; i < mesh->mNumVertices; i++)
//		{
//			Vertex vertex;
//			glm::vec3 vector;
//			// 位置坐标
//			vector.x = mesh->mVertices[i].x;
//			vector.y = mesh->mVertices[i].y;
//			vector.z = mesh->mVertices[i].z;
//			vertex.Position = vector;
//			// 发现坐标
//			vector.x = mesh->mNormals[i].x;
//			vector.y = mesh->mNormals[i].y;
//			vector.z = mesh->mNormals[i].z;
//			vertex.Normal = vector;
//			// 纹理坐标
//			if (mesh->mTextureCoords[0]) // 检测网格是否包含纹理坐标
//			{
//				glm::vec2 vec;
//				// 一个顶点理论上最多有8重纹理，不过我们这边假设只有一重，所以只判断第一重坐标是否存在
//				vec.x = mesh->mTextureCoords[0][i].x;
//				vec.y = mesh->mTextureCoords[0][i].y;
//				vertex.TexCoords = vec;
//			} else
//				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//			vertices.push_back(vertex);
//		}
//		// 遍历网格的每个面并且检索每个面的顶点索引
//		for (GLuint i = 0; i < mesh->mNumFaces; i++)
//		{
//			aiFace face = mesh->mFaces[i];
//			for (GLuint j = 0; j < face.mNumIndices; j++)
//				indices.push_back(face.mIndices[j]);
//		}
//		// 加工材质
//		if (mesh->mMaterialIndex >= 0)
//		{
//			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//			// 按照以下的格式加载纹理贴图
//			// Diffuse: texture_diffuseN
//			// Specular: texture_specularN
//			// Normal: texture_normalN
//
//			// 1. Diffuse maps
//			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//			// 2. Specular maps
//			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//		}
//
//
//		return Mesh(vertices, indices, textures);
//	}
//
//
//	// 加载纹理
//	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//	{
//		vector<Texture> textures;
//		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
//		{
//			aiString str;
//			mat->GetTexture(type, i, &str);
//			// 检查是否该纹理是否已经加载过，如果已经加载过就跳过它
//			GLboolean skip = false;
//			for (GLuint j = 0; j < textures_loaded.size(); j++)
//			{
//				if (textures_loaded[j].path == str)
//				{
//					textures.push_back(textures_loaded[j]);
//					skip = true;
//					break;
//				}
//			}
//			// 如果当前纹理没有加载过
//			if (!skip)
//			{
//				Texture texture;
//				texture.id = TextureFromFile(str.C_Str(), this->directory);
//				texture.type = typeName;
//				texture.path = str;
//				textures.push_back(texture);
//				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//			}
//		}
//		return textures;
//	}
//};
//
//
//
//
//GLint TextureFromFile(const char* path, string directory)
//{
//	//Generate texture ID and load texture data 
//	string filename = string(path);
//	filename = directory + '/' + filename;
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	int width, height;
//	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
//	// Assign texture to ID
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	SOIL_free_image_data(image);
//	return textureID;
//}

#pragma once
// std
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

// GL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
	/*  功能   */
	// 构造函数
	Model(GLchar* path)
	{
		this->loadModel(path);
	}

	// 绘制模型
	void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}

private:
	/*  模型数据  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// 存储已经加载的纹理，防止重复加载

										/*  功能   */
										// 用ASSIMP扩展加载模型并且把网格结果存储到mesh vector中
	void loadModel(string path)
	{
		// 用ASSIMP读取文件，并转换模型为三角形以及将图像沿y轴反转
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// 检查是否出错
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// 检索文件的目录路径
		this->directory = path.substr(0, path.find_last_of('/'));

		// 递归处理场景的节点
		this->processNode(scene->mRootNode, scene);
	}

	// 递归处理节点以及叶子节点
	void processNode(aiNode* node, const aiScene* scene)
	{
		// 处理当前节点下的各个网格
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// 处理叶子节点
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// 数据填充
		// 顶点
		vector<Vertex> vertices;
		// 索引
		vector<GLuint> indices;
		// 纹理
		vector<Texture> textures;

		// 遍历网格顶点
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			// 位置坐标
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// 发现坐标
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// 纹理坐标
			if (mesh->mTextureCoords[0]) // 检测网格是否包含纹理坐标
			{
				glm::vec2 vec;
				// 一个顶点理论上最多有8重纹理，不过我们这边假设只有一重，所以只判断第一重坐标是否存在
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			} else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// 遍历网格的每个面并且检索每个面的顶点索引
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// 加工材质
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// 按照以下的格式加载纹理贴图
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}


		return Mesh(vertices, indices, textures);
	}


	// 加载纹理
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// 检查是否该纹理是否已经加载过，如果已经加载过就跳过它
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			// 如果当前纹理没有加载过
			if (!skip)
			{
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};




GLint TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}