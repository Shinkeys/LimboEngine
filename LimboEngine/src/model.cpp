#include "../headers/model.h"

bool Model::loadModel(const char* path)
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
		
		/*////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
				████████╗███████╗██╗  ██╗████████╗██	██║ ██████╗	███████╗
				╚══██╔══╝██╔════╝██║  ██║╚══██╔══╝██	██║ ██╔══██╗██╔════╝
				   ██║   █████╗  ███████║   ██║   ██	██║ ██████╔╝█████╗ 
				   ██║   ██╔══╝  ██╔══██║   ██║   ██	██║ ██╔═══╝	██╔══╝ 
				   ██║   ███████╗██║  ██║   ██║   ██	██║ ██║		███████╗
				   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝	  ██████╚═╝ ╚═╝		╚══════╝
		
		/////////////////////////////////////////////////////////////////////////////*/

		else if (strcmp(lineHeader, "mtllib") == 0)
		{
			std::string fileName;

			iss >> fileName;
			m_mtlFileName.push_back(fileName);

			proceedMtlFile(fileName);
		}

		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			std::string mtlName;
			iss >> mtlName;
			if (checkForFirstUseMtlEntry)
			{
				fillMapWithKeysToDraw();
			}
			else if (m_vertexIndices.size() == 0)
			{
				checkForFirstUseMtlEntry = true;
			}

			m_usemtlLastName.push_back(mtlName);
		}

	}
	if (m_usemtlLastName.size() >= 1)
	{
		fillMapWithKeysToDraw();
	}


	m_outVertices.reserve(m_vertexIndices.size());
	m_outTextures.reserve(m_textureIndices.size());
	m_outNormal.reserve(m_normalIndices.size());

	for (unsigned int i = 0; i < m_vertexIndices.size(); ++i)
	{
		unsigned int vertexIndex = m_vertexIndices[i];
		unsigned int textureIndex = m_textureIndices[i];
		unsigned int normalIndex = m_normalIndices[i];
		

		m_outVertices.push_back(tempVertices[vertexIndex - 1]);
		m_outTextures.push_back(tempTextures[textureIndex - 1]);
		m_outNormal.push_back(tempNormal[normalIndex - 1]);

		m_outVertexIndices.push_back(i);
	}

	file.close();

	return true;
}
bool Model::proceedMtlFile(const std::string&  fileName)
{
	std::ifstream file;
	std::string pathToOpen = "../LimboEngine/Resources/objects/" + fileName;
	file.open(pathToOpen);
	if (!file)
	{
		std::cout << "Could not open the file by provided path!";
		return false;
	}
	std::string line;

	std::string tempMaterialName;
	
	MaterialsData tempMaterialsData;
	MaterialsPicturesData tempMaterialsPicFilesData;
	std::string tempMaterialPicFileName;
		
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

			tempMaterialsPicFilesData.diffuseMap = loadTextureFromFile(tempMaterialPicFileName);
		}
		else if (strcmp(lineHeader, "map_Ke") == 0)
		{
			iss >> tempMaterialPicFileName;

			tempMaterialsPicFilesData.emissionMap = loadTextureFromFile(tempMaterialPicFileName);
		}
		else if ((strcmp(lineHeader, "map_Bump") == 0) || (strcmp(lineHeader, "bump") == 0))
		{
			iss >> tempMaterialPicFileName;

			tempMaterialsPicFilesData.normalMap = loadTextureFromFile(tempMaterialPicFileName);
		}
		else
		{
			m_materialsValues.push_back(tempMaterialsData);
			m_materialsPicturesFilesFromMtl[tempMaterialName] = tempMaterialsPicFilesData;
		}
	}

}

unsigned int loadTextureFromFile(std::string& fileName)
{
	std::string pathToOpen = "../LimboEngine/Resources/objects/textures" + fileName;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathToOpen.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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








void Model::fillMapWithKeysToDraw()
{
	indexToDrawPart = m_vertexIndices.size();
	m_usemtlName[indexToDrawPart] = m_usemtlLastName[0];
	m_usemtlLastName.pop_back();
}