#include "../headers/displayModel.h"

void DisplayModel::setupOpenGLMeshData()
{
	std::cout << "\nMtlUseLastName size - " << model.getUseMtlNames().size() << '\n';
	std::cout << "indicesToDraw size - " << model.getUseMtlNames().size();
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	// to change glm::vec3 on MeshData
	glBufferData(GL_ARRAY_BUFFER, model.meshData.size() * sizeof(static_obj_loader::MeshData), &model.meshData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_outVertexIndices.size() * sizeof(unsigned int), 
		&model.m_outVertexIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	// to change glm::vec3 on MeshData
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(static_obj_loader::MeshData), (void*)0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(static_obj_loader::MeshData), (void*)offsetof(static_obj_loader::MeshData, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(static_obj_loader::MeshData), (void*)offsetof(static_obj_loader::MeshData, textures));

	glEnableVertexAttribArray(0);
}

void DisplayModel::Draw(Shader& shader)
{
	unsigned int offset = 0;
	unsigned int i = 0;
	std::string name;
	std::string currentNameOfMtlForCurrentIndices;
	for (const auto& x : model.m_indicesToDrawPart)
	{

		currentNameOfMtlForCurrentIndices = model.getUseMtlNames()[i];
		
		if (model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap) {
			name = "material.texture_diffuse";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].diffuseMap);
			++i;
		}
		if (model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap)
		{
			name = "material.texture_normal";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].normalMap);
			++i;
		}
		if (model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap)
		{
			name = "material.texture_emission";
			glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);
			glBindTextureUnit(i, model.m_materialsPicturesFilesFromMtlData[currentNameOfMtlForCurrentIndices].emissionMap);
			++i;
		}
	
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, x, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
		//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
		glBindVertexArray(0);

		offset = (unsigned int)x;
		
	}
}
