#include "../headers/model.h"
/*
   ____  ____       _
  / __ \|  _ \     | |
 | |  | | |_) |    | |
 | |  | |  _ < _   | |
 | |__| | |_) | |__| |
  \____/|____/ \____/
*/
namespace loader_constant_data
{
	bool proceedMtlFile(const std::filesystem::path& fileName)
	{
		std::ifstream file;
		std::filesystem::path pathToOpen = fileName;
		std::cout << "PATH TO OPEN: " << pathToOpen << '\n';
		file.open(pathToOpen.replace_extension(".mtl"));
		if (!file)
		{
			std::cout << "Could not open the texture file by provided path!";
			return false;
		}
		std::string line;

		std::string tempMaterialName;

		loader_constant_data::MaterialsData tempMaterialsData;
		loader_constant_data::MaterialsPicturesData tempMaterialsPicFilesData;
		std::string tempMaterialPicFileName = "";

		while (true)
		{
			char lineHeader[128];
			if (file.peek() == EOF)
			{
				break;
			}
			std::getline(file, line);

			std::istringstream iss(line);
			iss >> lineHeader;
			if (strcmp(lineHeader, "newmtl") == 0)
			{
				iss >> tempMaterialName;

			}
			else if (strcmp(lineHeader, "Ns") == 0)
			{
				iss >> tempMaterialsData.Ns;
			}
			else if (strcmp(lineHeader, "Ka") == 0)
			{
				iss >> tempMaterialsData.Ka.x
					>> tempMaterialsData.Ka.y
					>> tempMaterialsData.Ka.z;
			}
			else if (strcmp(lineHeader, "Kd") == 0)
			{
				iss >> tempMaterialsData.Kd.x
					>> tempMaterialsData.Kd.y
					>> tempMaterialsData.Kd.z;
			}
			else if (strcmp(lineHeader, "Ks") == 0)
			{
				iss >> tempMaterialsData.Ks.x
					>> tempMaterialsData.Ks.y
					>> tempMaterialsData.Ks.z;
			}
			else if (strcmp(lineHeader, "Ke") == 0)
			{
				iss >> tempMaterialsData.Ke.x
					>> tempMaterialsData.Ke.y
					>> tempMaterialsData.Ke.z;
			}
			else if (strcmp(lineHeader, "Ni") == 0)
			{
				iss >> tempMaterialsData.Ni;
			}
			else if (strcmp(lineHeader, "d") == 0)
			{
				iss >> tempMaterialsData.d;
			}
			else if (strcmp(lineHeader, "illum") == 0)
			{
				iss >> tempMaterialsData.illum;
			}

			////////////// material pictures files //////////////////
			else if (strcmp(lineHeader, "map_Kd") == 0)
			{
				iss >> tempMaterialPicFileName;

				tempMaterialsPicFilesData.diffuseMap = static_obj_loader::loadTextureFromFile(tempMaterialPicFileName, 1);
			}
			else if (strcmp(lineHeader, "map_Ke") == 0)
			{
				iss >> tempMaterialPicFileName;

				tempMaterialsPicFilesData.emissionMap = static_obj_loader::loadTextureFromFile(tempMaterialPicFileName);
			}
			else if (strcmp(lineHeader, "map_Ks") == 0)
			{
				iss >> tempMaterialPicFileName;

				tempMaterialsPicFilesData.specularMap = static_obj_loader::loadTextureFromFile(tempMaterialPicFileName);
			}
			else if ((strcmp(lineHeader, "map_Bump") == 0) || (strcmp(lineHeader, "bump") == 0))
			{
				iss >> tempMaterialPicFileName;

				tempMaterialsPicFilesData.normalMap = static_obj_loader::loadTextureFromFile(tempMaterialPicFileName);
			}
			else
			{
				/*m_materialsValues[indexToDrawPart] = tempMaterialsData;*/
				if (tempMaterialPicFileName != "" || ((tempMaterialPicFileName != "") && (file.peek() == EOF)))
				{
					convert_to_binary_pdd::materialsPicturesFilesFromMtlData[tempMaterialName] = tempMaterialsPicFilesData;
					std::cout << "Proceed mtl file data: " <<
						convert_to_binary_pdd::materialsPicturesFilesFromMtlData[tempMaterialName].diffuseMap << '\n';
				}
			}
		}
		return true;
	}
}
namespace static_obj_loader
{
	bool Model::loadModel(const std::filesystem::path& path)
	{
		std::ifstream file;
		file.open(path);
		if (!file)
		{
			std::cout << "Could not open the file by provided path!";
			return false;
		}

		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempTextures;
		std::vector<glm::vec3> tempNormal;

		std::string line;


		int i = 0;
		bool checkForFirstUseMtlEntry{ false };
		while (true)
		{
			char lineHeader[128];
			if (file.peek() == EOF)
			{
				break;
			}
			std::getline(file, line);

			std::istringstream iss(line);
			iss >> lineHeader;

			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				tempVertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 texture;
				iss >> texture.x >> texture.y;

				tempTextures.push_back(texture);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;

				tempNormal.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
				char slash;
				iss >> vertexIndex[0] >> slash >> textureIndex[0] >> slash >> normalIndex[0]
					>> vertexIndex[1] >> slash >> textureIndex[1] >> slash >> normalIndex[1]
					>> vertexIndex[2] >> slash >> textureIndex[2] >> slash >> normalIndex[2];
				if (!iss.eof())
				{
					std::cout << "This loader is unable right now to parse ur .obj file!";
					return false;
				}
				m_vertexIndices.push_back(vertexIndex[0]);
				m_vertexIndices.push_back(vertexIndex[1]);
				m_vertexIndices.push_back(vertexIndex[2]);
				m_textureIndices.push_back(textureIndex[0]);
				m_textureIndices.push_back(textureIndex[1]);
				m_textureIndices.push_back(textureIndex[2]);
				m_normalIndices.push_back(normalIndex[0]);
				m_normalIndices.push_back(normalIndex[1]);
				m_normalIndices.push_back(normalIndex[2]);
			}

			/*
			  _______ ________   _________ _    _ _____  ______  _____ 
			 |__   __|  ____\ \ / /__   __| |  | |  __ \|  ____|/ ____|
				| |  | |__   \ V /   | |  | |  | | |__) | |__  | (___  
				| |  |  __|   > <    | |  | |  | |  _  /|  __|  \___ \ 
				| |  | |____ / . \   | |  | |__| | | \ \| |____ ____) |
				|_|  |______/_/ \_\  |_|   \____/|_|  \_\______|_____/                     
			*/

			else if (strcmp(lineHeader, "mtllib") == 0)
			{
				std::string fileName;
				iss >> fileName;
				m_mtlFileName.push_back(fileName);

			}

			else if (strcmp(lineHeader, "usemtl") == 0)
			{
				std::string mtlName;
				iss >> mtlName;
				if (checkForFirstUseMtlEntry)
				{
					fillMapWithKeysToDraw(i);
					++i;
				}
				else if (m_vertexIndices.size() == 0)
				{
					checkForFirstUseMtlEntry = true;
				}

				m_usemtlNames.push_back(mtlName);
			}

		}
		if (m_indicesToDrawPart.size() < m_usemtlNames.size())
		{
			fillMapWithKeysToDraw(i);
			++i;
		}

		std::size_t sizeToReserve = m_vertexIndices.size() + m_textureIndices.size() + m_normalIndices.size();
		meshData.reserve(sizeToReserve);
		loader_constant_data::MeshData tempMeshData;
		for (unsigned int i = 0; i < m_vertexIndices.size(); ++i)
		{
			unsigned int vertexIndex = m_vertexIndices[i];
			unsigned int textureIndex = m_textureIndices[i];
			unsigned int normalIndex = m_normalIndices[i];

			tempMeshData.vertices = tempVertices[vertexIndex - 1];
			tempMeshData.textures = tempTextures[textureIndex - 1];
			tempMeshData.normal = tempNormal[normalIndex - 1];

			meshData.push_back(tempMeshData);

			m_outVertexIndices.push_back(i);
		}

		file.close();
		
		if (path.extension() == ".obj")
		{
			std::cout << "File type is - .obj\n";
			std::cout << "m_outVertexIndices.size(): " << m_outVertexIndices.size() << '\n';

			loader_constant_data::MeshDataCounts meshDataCounts;
			meshDataCounts.verticesCount = tempVertices.size();
			meshDataCounts.normalsCount = tempNormal.size();
			meshDataCounts.texturesCount = tempTextures.size();
			meshDataCounts.indicesCount = m_outVertexIndices.size();

			convert_to_binary_pdd::createPddFileFromObj(path, 
				meshDataCounts, meshData, m_indicesToDrawPart, m_usemtlNames);
		}

		return true;
	}

	unsigned int loadTextureFromFile(const std::string& fileName, int keyForDiffuseMapForGamma)
	{
		std::filesystem::path basePath = "../Resources/objects/textures/";
		std::filesystem::path pathToOpen = basePath / fileName;
		stbi_set_flip_vertically_on_load(true);
		std::cout << "Texture: " << fileName << '\n';
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(pathToOpen.string().c_str(), &width, &height, &nrChannels, 0);
		int format = nrChannels == 4 ? GL_RGBA : GL_RGB;
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			if (keyForDiffuseMapForGamma == 1)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Could not load textures to your model!";
		}
		stbi_image_free(data);

		return textureID;
	}

	void Model::fillMapWithKeysToDraw(int i)
	{
		m_indicesToDrawPart.push_back(m_vertexIndices.size());
		if (i >= 1)
		{
			m_indicesToDrawPart[i] -= m_indicesToDrawPart[i - 1];
		}
		// removing indices from previous iterations because we already drew em
	}
}
/*
  ____ _____ _   _          _______     __  ______ ____  _____  __  __       _______ 
 |  _ \_   _| \ | |   /\   |  __ \ \   / / |  ____/ __ \|  __ \|  \/  |   /\|__   __|
 | |_) || | |  \| |  /  \  | |__) \ \_/ /  | |__ | |  | | |__) | \  / |  /  \  | |   
 |  _ < | | | . ` | / /\ \ |  _  / \   /   |  __|| |  | |  _  /| |\/| | / /\ \ | |   
 | |_) || |_| |\  |/ ____ \| | \ \  | |    | |   | |__| | | \ \| |  | |/ ____ \| |   
 |____/_____|_| \_/_/    \_\_|  \_\ |_|    |_|    \____/|_|  \_\_|  |_/_/    \_\_|
*/
namespace convert_to_binary_pdd
{
	void createPddFileFromObj
	(
		const std::filesystem::path& fileName,
		const loader_constant_data::MeshDataCounts& meshDataCount,
		const std::vector<loader_constant_data::MeshData>& meshData, 
		const std::vector<unsigned int>& indicesToDrawPart,
		const std::vector<std::string>& model_useMtlNames)
	{
		std::filesystem::path currentFile = fileName;
		currentFile.replace_extension(".pdd");
		std::ofstream pddFile(currentFile, std::ios::binary);
		if (!pddFile)
		{
			std::cout << "Unable to proceed ur file!";
			return;
		}

		pddFile.write(reinterpret_cast<const char*>(&signature), sizeof(signature));
	
		// count of vertices
		pddFile.write(reinterpret_cast<const char*>(&meshDataCount.verticesCount), sizeof(meshDataCount.verticesCount));
		// count of normals
		pddFile.write(reinterpret_cast<const char*>(&meshDataCount.normalsCount), sizeof(meshDataCount.normalsCount));
		// count of textures
		pddFile.write(reinterpret_cast<const char*>(&meshDataCount.texturesCount), sizeof(meshDataCount.texturesCount));
		// count of indices to draw
		pddFile.write(reinterpret_cast<const char*>(&meshDataCount.indicesCount), sizeof(meshDataCount.indicesCount));

		//////// writing their data
		pddFile.write(reinterpret_cast<const char*>(meshData.data()),
			sizeof(loader_constant_data::MeshData) * meshData.size());
		
		unsigned int indicesToDrawPartSize = indicesToDrawPart.size();
		// writing indices to draw part size
		pddFile.write(reinterpret_cast<const char*>(&indicesToDrawPartSize), sizeof(indicesToDrawPartSize));
		// writing its data	
		pddFile.write(reinterpret_cast<const char*>(indicesToDrawPart.data()),
			sizeof(unsigned int) * indicesToDrawPart.size());
		

		// writing useMtlNames in a row which materials to use for each vertices
		unsigned int useMtlNamesSize = model_useMtlNames.size();
		pddFile.write(reinterpret_cast<const char*>(&useMtlNamesSize), sizeof(useMtlNamesSize));
		// writing its data

		for (unsigned int i = 0; i < useMtlNamesSize; ++i)
		{
			std::size_t size = model_useMtlNames[i].length();
			pddFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			pddFile.write(reinterpret_cast<const char*>(model_useMtlNames[i].c_str()), model_useMtlNames[i].length());
		}
		
		std::cout << "\nMeshDataSize: " << meshData.size() << '\n';
		////// writing indices count
		//std::cout << verticesCount << '\n';
		//std::cout << texturesCount << '\n';
		//std::cout << normalsCount << '\n';
		//std::cout << indicesCount << '\n';
		/*std::cout << reinterpret_cast<const char*>(&indicesCount), sizeof(verticesCount);*/

		pddFile.close();
	}

	void readPddFile(const std::filesystem::path& path, std::vector<PddMeshData>& pddMeshData)
	{
		std::ifstream pddFile(path, std::ios::binary);
		if (!pddFile)
		{
			std::cout << "File is not a .PDD!";
			std::abort();
		}


		char fileSignature[5];
		unsigned int verticesCount;
		unsigned int texturesCount;
		unsigned int normalsCount;
		unsigned int indicesCount;
		unsigned int indicesToDrawPartCount;
		unsigned int useMtlCount;

		pddFile.read(fileSignature, sizeof(signature));
		if (strcmp(fileSignature, "PIDD") != 0)
		{
			std::cout << "File header is not PIDD!!";
			std::abort();
		}

	
		
		pddFile.read(reinterpret_cast<char*>(&verticesCount), sizeof(verticesCount));
		std::cout << verticesCount << '\n';
		pddFile.read(reinterpret_cast<char*>(&normalsCount), sizeof(normalsCount));
		std::cout << normalsCount << '\n';
		pddFile.read(reinterpret_cast<char*>(&texturesCount), sizeof(texturesCount));
		std::cout << texturesCount << '\n';

		pddFile.read(reinterpret_cast<char*>(&indicesCount), sizeof(indicesCount));

		pddMeshData.resize(indicesCount);

		pddFile.read(reinterpret_cast<char*>(pddMeshData.data()),
			sizeof(convert_to_binary_pdd::PddMeshData) * indicesCount);

		pddFile.read(reinterpret_cast<char*>(&indicesToDrawPartCount), sizeof(indicesToDrawPartCount));
		
		indicesToDrawPart.resize(indicesToDrawPartCount);

		pddFile.read(reinterpret_cast<char*>(indicesToDrawPart.data()), 
			sizeof(unsigned int) * indicesToDrawPartCount);


		pddFile.read(reinterpret_cast<char*>(&useMtlCount), sizeof(useMtlCount));
		
		// reading strings of mtl names
		useMtlNames.resize(useMtlCount);

		for (unsigned int i = 0; i < useMtlCount; ++i)
		{
			std::size_t size;
			pddFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			useMtlNames[i].resize(size);
			pddFile.read(reinterpret_cast<char*>((char*)useMtlNames[i].c_str()), size);
		}

		outIndicesCount.reserve(indicesCount);


		for (auto i = 0; i < indicesCount; ++i)
		{
			outIndicesCount.push_back(i);
		}

		pddFile.close();

		// proceed MTL data
		loader_constant_data::proceedMtlFile(path);


	}

}