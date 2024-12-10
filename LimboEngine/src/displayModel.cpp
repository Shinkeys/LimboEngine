#include "../headers/displayModel.h"

void DisplayModel::setupOpenGLMeshData()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// to change glm::vec3 on MeshData
	glBufferData(GL_ARRAY_BUFFER, pddMeshData.size() * sizeof(convert_to_binary_pdd::PddMeshData), &pddMeshData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, convert_to_binary_pdd::outIndicesCount.size() * sizeof(unsigned int),
		&convert_to_binary_pdd::outIndicesCount[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	// to change glm::vec3 on MeshData
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)offsetof(convert_to_binary_pdd::PddMeshData, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)offsetof(convert_to_binary_pdd::PddMeshData, textures));

	glEnableVertexAttribArray(0);
}

void DisplayModel::Draw(Shader& shader)
{
	unsigned int offset = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	std::string name;
	std::string currentNameOfMtlForCurrentIndices;
	for (const auto& x : convert_to_binary_pdd::indicesToDrawPart)
	{
		currentNameOfMtlForCurrentIndices = convert_to_binary_pdd::useMtlNames[j];
		++j;

		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap) {
			name = "material.texture_diffuse";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap);
			++i;
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap)
		{
			name = "material.texture_normal";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap);
			++i;
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap)
		{
			name = "material.texture_emission";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap);
			++i;
		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, x, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
		//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
		glBindVertexArray(0);
	
		

		offset = (unsigned int)x;
		
	}
}
