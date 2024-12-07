#ifndef DISPLAY_H

#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/shader.h"
#include "../headers/model.h"

class DisplayModel
{
private:
	unsigned int VAO, VBO, EBO;
	static_obj_loader::Model model;
	void setupOpenGLMeshData();
public:
	DisplayModel(std::filesystem::path& path)
	{
		model.loadModel(path);
		setupOpenGLMeshData();
	}
	
	void Draw(Shader& shader);
};




#endif
