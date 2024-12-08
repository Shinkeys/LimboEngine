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
	std::vector<convert_to_binary_pdd::PddMeshData> pddMeshData;
	void setupOpenGLMeshData();
public:
	DisplayModel(std::filesystem::path& path)
	{
		if (path.extension() == ".obj")
		{
			model.loadModel(path);
		}
		else if (path.extension() == ".pdd")
		{
			convert_to_binary_pdd::readPddFile(path, pddMeshData);
		}
		
		setupOpenGLMeshData();
	}
	
	void Draw(Shader& shader);
};




#endif
