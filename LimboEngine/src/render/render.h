#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../Backend/OpenGL/openglBackend.h"
#include "../../headers/shader.h"
#include "../../headers/displayModel.h"

#include <iostream>
#include <filesystem>

enum class ObjectType
{
	SHAPE,
	MODEL,
	FLOOR
};






class OpenGLRender
{
private:
	glm::mat4 m_view{};
	glm::mat4 m_projection{};
	glm::mat4 m_lightProjectionMatrix{};
	
	GLuint m_depthMapFBO{};
	GLuint m_depthMap{};


	std::vector<DisplayModel> m_displayModel;
public:
	OpenGLRender(std::vector<DisplayModel>& modelsToLoadAndDisplay) :
		m_projection{ glm::perspective(glm::radians(45.0f),
			static_cast<float>(Default_Values::SCR_WIDTH) /
			static_cast<float>(Default_Values::SCR_HEIGHT), 0.1f, 100.0f) },
		m_displayModel{modelsToLoadAndDisplay}
	{

	}

	
	glm::mat4 makeLightProjectionMatrix();
	void renderToDepthMap();
	void clearBufferWithAttachedDepthMap();
	void generateShadowMapping();
	// simple 3d obj like cube
	bool fillLightProjectionMatrix(const Shader& shader);
	void drawSceneOfShadows(Shader& shader, const OpenGl_Backend& oglBackend, const ObjectType objectType);
	void drawSceneWithAttachedShadowMap(Shader& shader, 
		const OpenGl_Backend& oglBackend, const ObjectType objectType);
	void setShaderVariables(const Shader& shader, const ObjectType object);
	void bindDefaultFramebuffer();

	const auto getDepthMapFBO() const { return m_depthMapFBO; }
	const auto getDepthMap() const { return m_depthMap; }


	// others


};
void fillPerspectiveMatrix(PerspectiveValues& pValues);
