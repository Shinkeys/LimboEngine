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

#include <fstream>
#include <sstream>

struct Vertex
{
	glm::vec3 Vert;
	glm::vec3 Normal;
	glm::vec2 Uv;
};

struct Material
{
	std::string name;
	glm::vec3 ka;
	glm::vec3 kd;
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

	void Draw(Shader& shader)
	{
		unsigned int diffuseId = 1;
		unsigned int specularId = 1;
		unsigned int normalId = 1;

		unsigned int roughnessId = 1;
		
		for (unsigned int i = 0; i < texturesId.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textureName[i];
			if (name == "texture_diffuse")
				number = std::to_string(diffuseId++);
			else if (name == "texture_specular")
				number = std::to_string(specularId++); // transfer unsigned int to string
			else if (name == "texture_roughness")
				number = std::to_string(roughnessId++); 
			else if (name == "texture_normal")
				number = std::to_string(normalId++);

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, texturesId[i]);
		}

		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, vertexInd.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	} 
	
	

private:
	// processing data from the obj file
	Vertex vert;
	std::vector<Vertex> outVertices;
	std::vector<unsigned int> vertexInd, uvInd, normalInd;

	// textures loading
	std::string textureFile;
	std::vector<std::string> textureName;
	std::vector<unsigned int> texturesId;
	unsigned int index{ 0 };
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
		if (!myFile.is_open()) {
			std::cerr << "Failed to open file: " << pFile << std::endl;
			return;
		}
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempUvs;
		std::vector<glm::vec3> tempNormal; // storing all the data before pushing it to the main vector
		std::string line;
		while (myFile.peek() != EOF) {
			std::getline(myFile, line);
			if (line.starts_with("v"))
			{
				glm::vec3 vertex;
				std::istringstream lineStream(line.substr(2));
				lineStream >> vertex.x >> vertex.y >> vertex.z;

				tempVertices.emplace_back(vertex);
			}
			if (line.starts_with("vt"))
			{
				glm::vec2 uv;
				std::istringstream lineStream(line.substr(3));

				lineStream >> uv.x >> uv.y;

				tempUvs.emplace_back(uv);
			}
			if (line.starts_with("vn"))
			{	
				glm::vec3 normals;
				std::istringstream lineStream(line.substr(3));

				lineStream >> normals.x >> normals.y >> normals.z;
				tempNormal.emplace_back(normals);

			}
			if (line.starts_with("f"))
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
			if (line.starts_with("mtllib"))
			{
				std::istringstream lineStream(line.substr(6));

				lineStream >> textureFile;
			} // reading mtl name for textures
		}


		for (unsigned int i = 0; i < outVertices.size(); ++i)
		{
			vertexInd.emplace_back(i);
		} // adding indices of elements to for EBO

		processTextures();


		myFile.close();
	}
	

	void processTextures()
	{
		std::ifstream myFile;
		myFile.open(pathToTheFile());
		if (!myFile.is_open()) {
			std::cerr << "Failed to open file: " << textureFile << std::endl;
			return;
		}
		std::string line;
		Material material;
		std::string textureNameEntry;
		while (myFile.peek() != EOF) {
			std::getline(myFile, line);
			if (line.starts_with("newmtl"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> material.name;
			}
			if (line.starts_with("Ka"))
			{
				std::istringstream lineStream(line.substr(3));
				lineStream >> material.ka.x >> material.ka.y >> material.ka.z;
			}
			if (line.starts_with("Kd"))
			{
				std::istringstream lineStream(line.substr(3));
				lineStream >> material.kd.x >> material.kd.y >> material.kd.z;
			}
			if (line.starts_with("map_Kd"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> textureNameEntry;
				textureName.emplace_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}
			if (line.starts_with("map_Ks"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> textureNameEntry;
				textureName.push_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}
			if (line.starts_with("map_Ns"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> textureNameEntry;
				textureName.emplace_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}
			if (line.starts_with("bump"))
			{
				std::istringstream lineStream(line.substr(5));
				lineStream >> textureNameEntry;
				textureName.emplace_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}
			if (line.starts_with("map_Ao"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> textureNameEntry;
				textureName.emplace_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}
			if (line.starts_with("map_Pr"))
			{
				std::istringstream lineStream(line.substr(7));
				lineStream >> textureNameEntry;
				textureName.emplace_back(textureNameEntry);
				texturesId.emplace_back(getTextureFromFile(textureName[index++]));
			}

		}
		myFile.close();
	}


	unsigned int getTextureFromFile(const std::string& path)
	{
		std::string pathToTextures = std::string(path);
		//std::size_t found = pathToTextures.find_last_of("/");
		//pathToTextures.erase(found); // remove filename from path. Doesn't need for now !!!!!
		pathToTextures = "resources/objects/textures/" + pathToTextures;
		std::cout << pathToTextures << " ";
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrChannels;

		unsigned char* data = stbi_load(pathToTextures.c_str(), &width, &height, &nrChannels, 0);

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
	
	const std::string pathToTheFile()
	{
		return "resources/objects/" + textureFile;
	}

};


#endif