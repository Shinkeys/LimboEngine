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

namespace loader_constant_data
{
	struct MeshDataCounts
	{
		unsigned int verticesCount{};
		unsigned int normalsCount{};
		unsigned int texturesCount{};
		unsigned int indicesCount{};
	};
	struct MeshData
	{
		glm::vec3 vertices{};
		glm::vec3 normal{};
		glm::vec2 textures{};
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
		unsigned int specularMap{ 0 };
	};
	bool proceedMtlFile(const std::filesystem::path& fileName);
}

namespace static_obj_loader
{
	class Model
	{
	private:
		std::vector<unsigned int> m_vertexIndices, m_textureIndices, m_normalIndices;
		std::vector<std::string> m_usemtlNames;
		std::vector<std::string> m_mtlFileName;

	public:
		// mesh data
		std::vector<loader_constant_data::MeshData> meshData;

		std::vector<unsigned int> m_outVertexIndices;

		// indices to draw for current material
		std::vector<unsigned int> m_indicesToDrawPart;





		void fillMapWithKeysToDraw(int i);
		// get/set
		const auto getUseMtlNames() const { return m_usemtlNames; }
		const auto getAmountOfVertices() const { return m_vertexIndices; }
		const auto getAmountOfTextures() const { return m_textureIndices; }
		const auto getAmountOfNormals() const { return m_normalIndices; }



		bool loadModel(const std::filesystem::path& path);
	};
	unsigned int loadTextureFromFile(const std::string& path);
}


namespace convert_to_binary_pdd
{

	struct PddMeshData
	{
		glm::vec3 vertices{};
		glm::vec3 normal{};
		glm::vec2 textures{};
	};


	const char signature[] = "PIDD";


	inline std::unordered_map<std::string, loader_constant_data::MaterialsPicturesData> materialsPicturesFilesFromMtlData;
	inline std::vector<unsigned int> outIndicesCount;
	inline std::vector<unsigned int> indicesToDrawPart;
	inline std::vector<std::string> useMtlNames;

	void createPddFileFromObj(const std::filesystem::path& fileName,
		const loader_constant_data::MeshDataCounts& meshDataCount,
		const std::vector<loader_constant_data::MeshData>& meshData,
		const std::vector<unsigned int>& indicesToDrawPart,
		const std::vector<std::string>& model_useMtlNames);


	void readPddFile(const std::filesystem::path& path, std::vector<PddMeshData>& pddMeshData);
}

#endif