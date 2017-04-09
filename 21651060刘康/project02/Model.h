//#pragma once
//// C++��׼��
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//using namespace std;
//// GL��
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
//	/*  ����   */
//	// ���캯��
//	Model(GLchar* path)
//	{
//		this->loadModel(path);
//	}
//
//	// ����ģ��
//	void Draw(Shader shader)
//	{
//		for (GLuint i = 0; i < this->meshes.size(); i++)
//			this->meshes[i].Draw(shader);
//	}
//
//private:
//	/*  ģ������  */
//	vector<Mesh> meshes;
//	string directory;
//	vector<Texture> textures_loaded;	// �洢�Ѿ����ص�������ֹ�ظ�����
//
//										/*  ����   */
//										// ��ASSIMP��չ����ģ�Ͳ��Ұ��������洢��mesh vector��
//	void loadModel(string path)
//	{
//		// ��ASSIMP��ȡ�ļ�����ת��ģ��Ϊ�������Լ���ͼ����y�ᷴת
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//		// ����Ƿ����
//		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//		{
//			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
//			return;
//		}
//		// �����ļ���Ŀ¼·��
//		this->directory = path.substr(0, path.find_last_of('/'));
//
//		// �ݹ鴦�����Ľڵ�
//		this->processNode(scene->mRootNode, scene);
//	}
//
//	// �ݹ鴦��ڵ��Լ�Ҷ�ӽڵ�
//	void processNode(aiNode* node, const aiScene* scene)
//	{
//		// ����ǰ�ڵ��µĸ�������
//		for (GLuint i = 0; i < node->mNumMeshes; i++)
//		{
//			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//			this->meshes.push_back(this->processMesh(mesh, scene));
//		}
//		// ����Ҷ�ӽڵ�
//		for (GLuint i = 0; i < node->mNumChildren; i++)
//		{
//			this->processNode(node->mChildren[i], scene);
//		}
//
//	}
//
//	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//	{
//		// �������
//		// ����
//		vector<Vertex> vertices;
//		// ����
//		vector<GLuint> indices;
//		// ����
//		vector<Texture> textures;
//
//		// �������񶥵�
//		for (GLuint i = 0; i < mesh->mNumVertices; i++)
//		{
//			Vertex vertex;
//			glm::vec3 vector;
//			// λ������
//			vector.x = mesh->mVertices[i].x;
//			vector.y = mesh->mVertices[i].y;
//			vector.z = mesh->mVertices[i].z;
//			vertex.Position = vector;
//			// ��������
//			vector.x = mesh->mNormals[i].x;
//			vector.y = mesh->mNormals[i].y;
//			vector.z = mesh->mNormals[i].z;
//			vertex.Normal = vector;
//			// ��������
//			if (mesh->mTextureCoords[0]) // ��������Ƿ������������
//			{
//				glm::vec2 vec;
//				// һ�����������������8����������������߼���ֻ��һ�أ�����ֻ�жϵ�һ�������Ƿ����
//				vec.x = mesh->mTextureCoords[0][i].x;
//				vec.y = mesh->mTextureCoords[0][i].y;
//				vertex.TexCoords = vec;
//			} else
//				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//			vertices.push_back(vertex);
//		}
//		// ���������ÿ���沢�Ҽ���ÿ����Ķ�������
//		for (GLuint i = 0; i < mesh->mNumFaces; i++)
//		{
//			aiFace face = mesh->mFaces[i];
//			for (GLuint j = 0; j < face.mNumIndices; j++)
//				indices.push_back(face.mIndices[j]);
//		}
//		// �ӹ�����
//		if (mesh->mMaterialIndex >= 0)
//		{
//			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//			// �������µĸ�ʽ����������ͼ
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
//	// ��������
//	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//	{
//		vector<Texture> textures;
//		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
//		{
//			aiString str;
//			mat->GetTexture(type, i, &str);
//			// ����Ƿ�������Ƿ��Ѿ����ع�������Ѿ����ع���������
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
//			// �����ǰ����û�м��ع�
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
	/*  ����   */
	// ���캯��
	Model(GLchar* path)
	{
		this->loadModel(path);
	}

	// ����ģ��
	void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}

private:
	/*  ģ������  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// �洢�Ѿ����ص�������ֹ�ظ�����

										/*  ����   */
										// ��ASSIMP��չ����ģ�Ͳ��Ұ��������洢��mesh vector��
	void loadModel(string path)
	{
		// ��ASSIMP��ȡ�ļ�����ת��ģ��Ϊ�������Լ���ͼ����y�ᷴת
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// ����Ƿ����
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// �����ļ���Ŀ¼·��
		this->directory = path.substr(0, path.find_last_of('/'));

		// �ݹ鴦�����Ľڵ�
		this->processNode(scene->mRootNode, scene);
	}

	// �ݹ鴦��ڵ��Լ�Ҷ�ӽڵ�
	void processNode(aiNode* node, const aiScene* scene)
	{
		// ����ǰ�ڵ��µĸ�������
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// ����Ҷ�ӽڵ�
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// �������
		// ����
		vector<Vertex> vertices;
		// ����
		vector<GLuint> indices;
		// ����
		vector<Texture> textures;

		// �������񶥵�
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			// λ������
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// ��������
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// ��������
			if (mesh->mTextureCoords[0]) // ��������Ƿ������������
			{
				glm::vec2 vec;
				// һ�����������������8����������������߼���ֻ��һ�أ�����ֻ�жϵ�һ�������Ƿ����
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			} else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// ���������ÿ���沢�Ҽ���ÿ����Ķ�������
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// �ӹ�����
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// �������µĸ�ʽ����������ͼ
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


	// ��������
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// ����Ƿ�������Ƿ��Ѿ����ع�������Ѿ����ع���������
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
			// �����ǰ����û�м��ع�
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