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
	std::vector<convert_to_binary_pdd::PddMeshData> pddMeshData;
	void setupOpenGLMeshData();
public:
	DisplayModel(std::filesystem::path& path)
	{
		if (path.extension() == ".obj")
		{
			static_obj_loader::Model model;
			model.loadModel(path);
			std::cout << "CONVERTED MODEL TO BINARY! REPLACE PATH ON THE NEW FILE";
			std::exit(EXIT_SUCCESS);
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
