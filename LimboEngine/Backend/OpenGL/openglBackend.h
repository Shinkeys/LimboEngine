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
	// shadow map
	extern const unsigned int SHADOW_WIDTH;
	extern const unsigned int SHADOW_HEIGHT;
	// movement
	extern float deltaTime;
	extern float lastTime;

	extern bool firstMove;
	extern float lastX;
	extern float lastY;

	extern glm::vec3 lightPos;

	// camera
	extern Camera camera;

	extern float zFar;
	extern float zNear;


}
class OpenGl_Backend
{
private:
	GLuint m_vertexArray{ 0 };
	GLuint m_vertexBuffer{ 0 };
	GLuint m_elementBuffer{ 0 };


public:
	int initialization();
	void setupUnskinnedByVBO(std::vector<float>& vertices);
	void setupUnskinnedByEBO(std::vector<float>& vertices, std::vector<uint32_t>& indices);

	void deleteGlData();

	void lightMove(GLFWwindow* window);


	GLuint getVertexVAO() const
	{
		return m_vertexArray;
	}
	GLuint getVertexVBO() const
	{
		return m_vertexBuffer;
	}
	GLuint getElementBuffer() const 
	{
		return m_elementBuffer;
	}
};
static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double x, double y);


// clamp
template <typename T>
const T& clamp(const T& number, const T& min, const T& max)
{
	return std::min(max, std::max(number, min));
}







#endif // !OPENGL_H