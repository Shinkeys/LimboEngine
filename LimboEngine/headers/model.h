﻿#ifndef MODEL_H

#define MODEL_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


#include "../headers/stb_image.h"

#include <optional>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>



struct MeshData
{
	glm::vec3 vertices{};
	glm::vec2 textures{};
	glm::vec3 normal{};
};


struct MaterialsData
{
	glm::vec3 Ka{ 0.0 };
	glm::vec3 Kd{ 0.0 };
	glm::vec3 Ks{ 0.0 };
	glm::vec3 Ke{ 0.0 };
	float Ns{ 0.0 };
	float Ni{ 0.0 };
	float d{ 0.0 };
	int illum{ 0 };
};

struct MaterialsPicturesData
{
	unsigned int diffuseMap{0};
	unsigned int emissionMap{0};
	unsigned int normalMap{0};
};


class Model
{
private:
	
public:
	// mesh data
	std::vector<MeshData> meshData;

	std::vector<unsigned int> m_vertexIndices, m_textureIndices, m_normalIndices;
	std::vector<unsigned int> m_outVertexIndices;

	// textures
	std::size_t indexToDrawPart;
	std::map<std::size_t, std::string> m_usemtlName;	
	std::string m_usemtlLastName;
	std::vector<std::string> m_mtlFileName;
	std::map<std::string, MaterialsData> m_materialsValues;
	void fillMapWithKeysToDraw();
	bool proceedMtlFile(const std::string& fileName);

	std::map<std::string, MaterialsPicturesData> m_materialsPicturesFilesFromMtlData;





	bool loadModel(const char* path);
};

unsigned int loadTextureFromFile(std::string& path);



#endif