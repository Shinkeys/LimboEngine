#ifndef MODEL_H

#define MODEL_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


#include "../headers/stb_image.h"

#include <filesystem>
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
		std::vector<unsigned int> m_vertexIndices, m_textureIndices, m_normalIndices;
		std::vector<std::string> m_usemtlNames;
		std::vector<std::string> m_mtlFileName;

	public:
		// mesh data
		std::vector<MeshData> meshData;

		std::vector<unsigned int> m_outVertexIndices;

		// indices to draw for current material
		std::vector<unsigned int> m_indicesToDrawPart;



		std::unordered_map<unsigned int, MaterialsData> m_materialsValues;
		// hold indices of textures loaded by stbi_load for current usemtl
		std::unordered_map<std::string, MaterialsPicturesData> m_materialsPicturesFilesFromMtlData;


		void fillMapWithKeysToDraw(int i);
		bool proceedMtlFile(const std::string& fileName);
		// get/set
		const auto getUseMtlNames() const { return m_usemtlNames; }




		bool loadModel(const std::filesystem::path& path);
	};
	unsigned int loadTextureFromFile(const std::string& path);
}


#endif