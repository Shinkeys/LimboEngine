#include "../headers/displayModel.h"

void DisplayModel::setupOpenGLMeshData()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	
	glBufferData(GL_ARRAY_BUFFER, pddMeshData.size() * sizeof(convert_to_binary_pdd::PddMeshData), &pddMeshData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, convert_to_binary_pdd::outIndicesCount.size() * sizeof(unsigned int),
		&convert_to_binary_pdd::outIndicesCount[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)offsetof(convert_to_binary_pdd::PddMeshData, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(convert_to_binary_pdd::PddMeshData), (void*)offsetof(convert_to_binary_pdd::PddMeshData, textures));

	glEnableVertexAttribArray(0);
}

void DisplayModel::Draw(const Shader& shader)
{
	unsigned int offset = 0;
	unsigned int j = 0;
	std::string name;
	std::string currentNameOfMtlForCurrentIndices;
	for (const auto& x : convert_to_binary_pdd::indicesToDrawPart)
	{
		if (j < convert_to_binary_pdd::useMtlNames.size())
		{
			currentNameOfMtlForCurrentIndices = convert_to_binary_pdd::useMtlNames[j];
			++j;
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap) {
			name = "material.texture_diffuse";
			glUniform1i(shader.materialUniforms.diffuseLocation, 0);
			glBindTextureUnit(0, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap);
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap)
		{
			name = "material.texture_normal";
			glUniform1i(shader.materialUniforms.normalLocation, 1);
			glBindTextureUnit(1, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap);
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap)
		{
			name = "material.texture_emission";
			glUniform1i(shader.materialUniforms.emissionLocation, 2);
			glBindTextureUnit(2, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap);
		}
		if (convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].specularMap)
		{
			name = "material.texture_specular";
			glUniform1i(shader.materialUniforms.specularLocation, 3);
			glBindTextureUnit(3, convert_to_binary_pdd::materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].specularMap);
		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, x, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
		//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
		glBindVertexArray(0);
		offset = (unsigned int)x;
	}
	
}
