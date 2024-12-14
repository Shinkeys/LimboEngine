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
	std::vector<uint32_t> indices;
};


int main() {
	OpenGl_Backend oglBackend;
	oglBackend.initialization();
	OpenGLRender oglRender;

	// personal settings to replace later
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	Shader lamp("shaders/lamp.vert", "shaders/lamp.frag");
	Shader character("shaders/character.vert", "shaders/character.frag");
	Shader shadows("shaders/shadowDepth.vert", "shaders/shadowDepth.frag");
	Meshes mesh;
	
	std::vector<float> verticesUntextured {
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

	mesh.indices = {
	0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
	};


	// creating and filling buffers 
	oglBackend.setupUnskinnedByEBO(verticesUntextured, mesh.indices);



	character.initializeUniformData();
	

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


		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shadows.use();
		oglRender.drawSceneOfShadows(shadows, oglBackend);

		character.use();
		
		lamp.use();

		glfwSwapBuffers(Default_Values::window);
		glfwPollEvents();
	}
	oglBackend.deleteGlData();


	return 0;
}
