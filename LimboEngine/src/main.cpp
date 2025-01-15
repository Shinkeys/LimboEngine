#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/displayModel.h"
#include "../Backend/OpenGL/openglBackend.h"
#include "../src/render/render.h"



#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"



struct Meshes {
	std::vector<uint32_t> indicesCube;
	std::vector<uint32_t> indicesSquare;
};

int main() {
	OpenGl_Backend oglBackend;
	oglBackend.initialization();

	std::filesystem::path pathToTheModel = "Resources/objects/character.pdd";
	DisplayModel dModel(pathToTheModel);
	std::vector<DisplayModel> modelsToDisplay;
	modelsToDisplay.push_back(dModel);

	OpenGLRender oglRender(modelsToDisplay);

	// personal settings to replace later
	
	// for grid
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW);
	Shader lamp("shaders/lamp.vert", "shaders/lamp.frag");
	Shader character("shaders/character.vert", "shaders/character.frag");
	Shader grid("shaders/grid.vert", "shaders/grid.frag");
	Meshes mesh;

	
	std::vector<float> verticesUntexturedCube {
					// normals            // texture
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
						   
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
						   
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
						   
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
						   
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
						   
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};					   

	mesh.indicesCube = {
	0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
	};


	// creating and filling buffers 
	oglBackend.setupUnskinnedByEBO(verticesUntexturedCube, mesh.indicesCube);



	character.initializeUniformData();
	grid.initializeShadowMapShader();
	

	// generation of map of shadows
	oglRender.generateShadowMapping();


	// render loop
	// -----------
	while (!glfwWindowShouldClose(Default_Values::window))
	{
		// input
		// -----
		processInput(Default_Values::window);
		oglBackend.lightMove(Default_Values::window);



		float currentTime = (float)glfwGetTime();

		Default_Values::deltaTime = currentTime - Default_Values::lastTime;

		Default_Values::lastTime = currentTime;

		// depth map drawing
		oglRender.renderToDepthMap();
		oglRender.drawSceneOfShadows(lamp, oglBackend, ObjectType::SHAPE);
		oglRender.drawSceneOfShadows(grid, oglBackend, ObjectType::FLOOR);
		oglRender.drawSceneOfShadows(character, oglBackend, ObjectType::MODEL);
		oglRender.bindDefaultFramebuffer();
		////// ready map drawing
		oglRender.clearBufferWithAttachedDepthMap();
		oglRender.drawSceneWithAttachedShadowMap(lamp, oglBackend, ObjectType::SHAPE);
		oglRender.drawSceneWithAttachedShadowMap(grid, oglBackend, ObjectType::FLOOR);
		oglRender.drawSceneWithAttachedShadowMap(character, oglBackend, ObjectType::MODEL);



		glfwSwapBuffers(Default_Values::window);
		glfwPollEvents();
	}
	oglBackend.deleteGlData();


	return 0;
}
