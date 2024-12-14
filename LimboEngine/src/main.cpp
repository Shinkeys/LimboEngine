#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/displayModel.h"
#include "../Backend/OpenGL/openglBackend.h"

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

	// personal settings to replace later
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	Shader lamp("shaders/lamp.vert", "shaders/lamp.frag");
	Shader character("shaders/character.vert", "shaders/character.frag");
	std::filesystem::path pathToTheModel = "../LimboEngine/Resources/objects/character.pdd";
	/*std::filesystem::path pathToTheModel = "../LimboEngine/Resources/objects/FragataVictoria.obj";*/
	DisplayModel dModel(pathToTheModel);
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

		character.use();
		character.setVec3("lightPos", Default_Values::lightPos);


		glm::mat4 view = Default_Values::camera.getViewMatrix();
		character.setMat4("view", view);



		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
			static_cast<float>(Default_Values::SCR_WIDTH) / 
			static_cast<float>(Default_Values::SCR_HEIGHT), 0.1f, 100.0f);

		character.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		character.setMat4("model", model);
		dModel.Draw(character);

		lamp.use();


		model = glm::translate(model, Default_Values::lightPos);
		model = glm::scale(model, glm::vec3(0.3f));
		lamp.setMat4("model", model);
		lamp.setMat4("view", view);
		lamp.setMat4("projection", projection);



		glBindVertexArray(oglBackend.getVertexVAO());
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(Default_Values::window);
		glfwPollEvents();
	}
	oglBackend.deleteGlData();


	return 0;
}
