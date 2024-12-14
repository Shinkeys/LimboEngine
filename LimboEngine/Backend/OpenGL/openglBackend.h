#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../headers/camera.h"

#include <iostream>
#include <vector>

namespace OpenGL_Backend
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam);
	void processInputMode_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double xPos, double yPos);

	GLuint getVertexVAO();
	GLuint getVertexVBO();
	GLuint getElementBuffer();

	int initialization();

	void drawUnskinnedByVBO(std::vector<float>& vertices);
	void drawUnskinnedByEBO(std::vector<float>& vertices, std::vector<uint32_t>& indices);

}
