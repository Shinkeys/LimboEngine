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
	unsigned int i = 0;
	unsigned int diffuseNr = 1;
	unsigned int emissionNr = 1;
	unsigned int normalNr = 1;
	for (const auto& x : model.m_usemtlName)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name = "texture_diffuse";
		glUniform1i(glGetUniformLocation(shader.ID, (name + std::to_string(diffuseNr++)).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, model.m_materialsPicturesFilesFromMtlData[x.second].diffuseMap);
		++i;

		glActiveTexture(GL_TEXTURE0 + i);
		name = "texture_normal";
		glUniform1i(glGetUniformLocation(shader.ID, (name + std::to_string(normalNr++)).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, model.m_materialsPicturesFilesFromMtlData[x.second].normalMap);
		++i;
		/*std::string name = "texture_diffuse";
		glUniform1i(glGetUniformLocation(shader.ID, (name + std::to_string(diffuseNr)).c_str()), i);*/









		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)model.m_outVertexIndices.size(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
		glBindVertexArray(0);

	}
}