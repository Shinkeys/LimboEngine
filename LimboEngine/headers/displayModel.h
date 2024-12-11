#ifndef DISPLAY_H

#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/shader.h"
#include "../headers/model.h"

class DisplayModel
{
private:
	unsigned int VAO{}, VBO{}, EBO{};
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

		}
		else if (path.extension() == ".pdd")
		{
			convert_to_binary_pdd::readPddFile(path, pddMeshData);
		}
		
		setupOpenGLMeshData();
	}
	~DisplayModel()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
		pddMeshData.clear();
	}
	DisplayModel(DisplayModel&& obj) noexcept : pddMeshData{ obj.pddMeshData }
	{
		obj.pddMeshData.clear();
	}
	void Draw(Shader& shader);
};
#endif
