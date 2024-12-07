#include "../headers/displayModel.h"

void DisplayModel::setupOpenGLMeshData(std::size_t indicesToDraw)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	// to change glm::vec3 on MeshData
	glBufferData(GL_ARRAY_BUFFER, model.meshData.size() * sizeof(MeshData), &model.meshData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_outVertexIndices.size() * sizeof(unsigned int), 
		&model.m_outVertexIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	// to change glm::vec3 on MeshData
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)offsetof(MeshData, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)offsetof(MeshData, textures));

	glEnableVertexAttribArray(0);
}

void DisplayModel::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int emissionNr = 1;
	unsigned int normalNr = 1;
	unsigned int offset = 0;
	unsigned int i = 0;
	std::string name;
	for (const auto& x : model.m_usemtlName)
	{
		std::cout << "DiffuseMap ID: " << model.m_materialsPicturesFilesFromMtlData[x.second].diffuseMap << "\n";
		std::cout << "NormalMap ID: " << model.m_materialsPicturesFilesFromMtlData[x.second].normalMap << "\n";

		if (model.m_materialsPicturesFilesFromMtlData[x.second].diffuseMap) {
			name = "material.texture_diffuse1";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, model.m_materialsPicturesFilesFromMtlData[x.second].diffuseMap);
			++diffuseNr;
			++i;
		}
		/*if (model.m_materialsPicturesFilesFromMtlData[x.second].normalMap)
		{
			name = "material.texture_normal1";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, model.m_materialsPicturesFilesFromMtlData[x.second].normalMap);
			++i;
		}*/

	
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, x.first, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
		//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
		glBindVertexArray(0);

		offset = (unsigned int)x.first;
		
	}
}
		/*std::string name = "texture_diffuse";
		glUniform1i(glGetUniformLocation(shader.ID, (name + std::to_string(diffuseNr)).c_str()), i);*/
