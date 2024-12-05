#include "../headers/displayModel.h"

void DisplayModel::setupOpenGLMeshData(std::size_t indicesToDraw)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	// to change glm::vec3 on MeshData
	glBufferData(GL_ARRAY_BUFFER, indicesToDraw * sizeof(glm::vec3), &model.m_usemtlName[indicesToDraw], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesToDraw * sizeof(unsigned int), &model.m_outVertexIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	// to change glm::vec3 on MeshData
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);


	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)offsetof(MeshData, normalCoordinates));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData), (void*)offsetof(MeshData, textureCoordinates));*/

	glEnableVertexAttribArray(0);
}

void DisplayModel::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)model.m_outVertexIndices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, model.m_outVertices.size());
	glBindVertexArray(0);
}