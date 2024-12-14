#ifndef OPENGL_H
#define OPENGL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../headers/camera.h"

#include <iostream>
#include <vector>

namespace Default_Values
{
	// window
	extern GLFWwindow* window;
	// settings
	extern const unsigned int SCR_WIDTH;
	extern const unsigned int SCR_HEIGHT;
	// movement
	extern float deltaTime;
	extern float lastTime;

	extern bool firstMove;
	extern float lastX;
	extern float lastY;

	extern glm::vec3 lightPos;

	// camera
	extern Camera camera;


}
class OpenGl_Backend
{
private:
	GLuint g_vertexArray{ 0 };
	GLuint g_vertexBuffer{ 0 };
	GLuint g_elementBuffer{ 0 };


public:
	int initialization();
	void setupUnskinnedByVBO(std::vector<float>& vertices);
	void setupUnskinnedByEBO(std::vector<float>& vertices, std::vector<uint32_t>& indices);

	void deleteGlData();

	void lightMove(GLFWwindow* window);


	GLuint getVertexVAO()
	{
		return g_vertexArray;
	}
	GLuint getVertexVBO()
	{
		return g_vertexBuffer;
	}
	GLuint getElementBuffer()
	{
		return g_elementBuffer;
	}
};
static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);
void processInputMode_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
#endif // !OPENGL_H