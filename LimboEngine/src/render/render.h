#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../Backend/OpenGL/openglBackend.h"
#include "../../headers/shader.h"
#include "../../headers/displayModel.h"

#include <iostream>
#include <filesystem>

enum class DrawingObjectType
{
	SHAPE,
	MODEL
};

class OpenGLRender
{
private:
	glm::mat4 m_model{};
	glm::mat4 m_view{};
	glm::mat4 m_projection{};
	glm::mat4 m_lightProjectionMatrix{};
	
	GLuint m_depthMapFBO{};
	GLuint m_depthMap{};
public:
	OpenGLRender() : m_model{ glm::mat4(1.0f) },
		m_view{ glm::perspective(glm::radians(45.0f),
			static_cast<float>(Default_Values::SCR_WIDTH) /
			static_cast<float>(Default_Values::SCR_HEIGHT), 0.1f, 100.0f) }
	{

	}

	
	glm::mat4& makeLightProjectionMatrix();
	void renderToDepthMap();
	void renderWithAttachedDepthMap();
	void generateShadowMapping();
	// simple 3d obj like cube
	bool drawSceneOfShadows(const Shader& shader, const OpenGl_Backend& oglBackend);
	bool drawSceneWithAttachedShadowMap(const Shader& shader, 
		const OpenGl_Backend& oglBackend, DrawingObjectType objectType);




	const auto getDepthMapFBO() const { return m_depthMapFBO; }
	const auto getDepthMap() const { return m_depthMap; }


};
