#ifndef MODEL_H

#define MODEL_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


#include "../headers/stb_image.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


namespace static_obj_loader
{
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
		unsigned int diffuseMap{ 0 };
		unsigned int emissionMap{ 0 };
		unsigned int normalMap{ 0 };
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
		std::vector<unsigned int> m_indicesToDrawPart;
		std::vector<std::string> m_usemtlNames;
		std::vector<std::string> m_mtlFileName;
		std::unordered_map<unsigned int, MaterialsData> m_materialsValues;
		void fillMapWithKeysToDraw(int i);
		bool proceedMtlFile(const std::string& fileName);

		// hold indices of textures loaded by stbi_load for current usemtl
		std::unordered_map<std::string, MaterialsPicturesData> m_materialsPicturesFilesFromMtlData;





		bool loadModel(const char* path);
	};

	unsigned int loadTextureFromFile(std::string& path);
}


#endif