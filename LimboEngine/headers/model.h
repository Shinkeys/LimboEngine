#ifndef MODEL_H
#define MODEL_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../headers/shader.h"
#include <string>
#include <vector>

#include "../headers/stb_image.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <set>
#include <fstream>
#include <sstream>

struct Vertex
{
	glm::vec3 Vert;
	glm::vec3 Normal;
	glm::vec2 Uv;
};

class Model
{
public:

	unsigned int VAO;

	Model(const std::string& path)
	{
		processMesh(path);
		setupMesh(vert);
	}

	void Draw()
	{
		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, vertexInd.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	} 
	
	

private:
	Vertex vert;
	std::vector<Vertex> outVertices;
	std::vector<unsigned int> vertexInd, uvInd, normalInd;
	unsigned int VBO, EBO;

	void setupMesh(const Vertex& vert)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(Vertex), &outVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexInd.size() * sizeof(unsigned int), &vertexInd[0], GL_STATIC_DRAW); // to do

		
	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uv));

		

		glBindVertexArray(0);

	}

	void processMesh(const std::string& pFile)
	{
		std::ifstream myFile;
		myFile.open(pFile);
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempUvs;
		std::vector<glm::vec3> tempNormal;
		std::string line;

		while (myFile.peek() != EOF) {
			std::getline(myFile, line);
			if (line.rfind("v", 0) == 0)
			{
				glm::vec3 vertex;
				std::istringstream lineStream(line.substr(2));
				lineStream >> vertex.x >> vertex.y >> vertex.z;

				tempVertices.emplace_back(vertex);
			}
			if (line.compare(0, 2, "vt") == 0)
			{
				glm::vec2 uv;
				std::istringstream lineStream(line.substr(3));

				lineStream >> uv.x >> uv.y;

				tempUvs.emplace_back(uv);
			}
			if (line.compare(0, 2, "vn") == 0)
			{	
				glm::vec3 normals;
				std::istringstream lineStream(line.substr(3));

				lineStream >> normals.x >> normals.y >> normals.z;
				tempNormal.emplace_back(normals);

			}
			if (line.rfind("f", 0) == 0)
			{
				unsigned int vertex[3], uv[3], normal[3];
				std::istringstream lineStream(line.substr(2));
				char slash;
				



				for (int i = 0; i < 3; ++i) {
					lineStream >> vertex[i] >> slash >> uv[i] >> slash >> normal[i];
					// adding vertices by faces to draw
					vert.Vert = tempVertices[vertex[i] - 1];
					// adding normals
					vert.Normal = tempNormal[normal[i] - 1];
					vert.Uv = tempUvs[uv[i] - 1];
					//// adding texture coords


					outVertices.emplace_back(vert);
				}
			}
		}
		
		for (unsigned int i = 0; i < outVertices.size(); ++i)
		{
			vertexInd.emplace_back(i);
		} // adding indices of elements to for EBO
		
			
	}
	

	





























	unsigned int getTextureFromFile(const char* path, const std::string& directory)
	{

		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		std::cout << filename;
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrChannels;

		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			GLenum format;
			if (nrChannels == 1)
			{
				format = GL_RED;
			}
			else if (nrChannels == 3)
			{
				format = GL_RGB;
			}
			else if (nrChannels == 4)
			{
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
		else
		{
			std::cout << "Failed to load textures" << std::endl;
		}
		stbi_image_free(data);
		return textureID;
	}

};


#endif