#ifndef DISPLAY_H

#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "../headers/model.h"

class DisplayModel
{
private:
	unsigned int VAO, VBO, EBO;
	Model model;
	void setupOpenGLMeshData(std::size_t indicesToDraw);
public:
	DisplayModel(const char* path)
	{
		model.loadModel(path);
		setupOpenGLMeshData(model.m_vertexIndices.size());
	}
	
	void Draw();
};




#endif
