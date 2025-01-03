#include "render.h"



glm::mat4 OpenGLRender::makeLightProjectionMatrix()
{
	float nearPlane = 1.0f;
	float farPlane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(Default_Values::lightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjectionMatrix = lightProjection * lightView;

	
	return lightProjectionMatrix;
}
void OpenGLRender::renderToDepthMap()
{
	glViewport(0, 0, Default_Values::SHADOW_WIDTH, Default_Values::SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void OpenGLRender::clearBufferWithAttachedDepthMap()
{
	glViewport(0, 0, Default_Values::SCR_WIDTH, Default_Values::SCR_HEIGHT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
}

void OpenGLRender::generateShadowMapping()
{
	glGenFramebuffers(1, &m_depthMapFBO);


	glGenTextures(1, &m_depthMap);

	glBindTexture(GL_TEXTURE_2D, m_depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		Default_Values::SHADOW_WIDTH, Default_Values::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool OpenGLRender::fillLightProjectionMatrix(Shader& shader,
	const OpenGl_Backend& oglBackend)
{
	shader.use();

	m_lightProjectionMatrix = makeLightProjectionMatrix();
	
	glm::mat4 model(1.0f);

	shader.setMat4("lightSpaceMatrix", m_lightProjectionMatrix);

	shader.setMat4("model", model);


	return true;
}

void OpenGLRender::drawSceneOfShadows(Shader& shader, const OpenGl_Backend& oglBackend, const DrawingObjectType objectType)
{
	shader.use();

	if (objectType == DrawingObjectType::SHAPE)
	{
		glBindVertexArray(oglBackend.getVertexVAO());
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else if (objectType == DrawingObjectType::FLOOR)
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	else if (objectType == DrawingObjectType::MODEL)
	{
		for (auto i = 0; i < m_displayModel.size(); ++i)
		{
			m_displayModel[i].Draw(shader);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRender::drawSceneWithAttachedShadowMap(Shader& shader,
	const OpenGl_Backend& oglBackend, const DrawingObjectType objectType)
{
	shader.use();

	if (objectType == DrawingObjectType::SHAPE)
	{
		setShaderVariables(shader, objectType);

		glBindVertexArray(oglBackend.getVertexVAO());
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
	else if (objectType == DrawingObjectType::FLOOR)
	{
		setShaderVariables(shader, objectType);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else if (objectType == DrawingObjectType::MODEL)
	{
		
		setShaderVariables(shader, objectType);

		for (auto i = 0; i < m_displayModel.size(); ++i)
		{
			m_displayModel[i].Draw(shader, m_depthMap);
		}
	}

}


void OpenGLRender::setShaderVariables(const Shader& shader, const DrawingObjectType object)
{
	if (object == DrawingObjectType::SHAPE)
	{
		glm::mat4 model(1.0f);
		m_view = Default_Values::camera.getViewMatrix();
		model = glm::translate(model, Default_Values::lightPos);
		model = glm::scale(model, glm::vec3(0.3f));
		shader.setMat4("model", model);
		shader.setMat4("view", m_view);
		shader.setMat4("projection", m_projection);

	}
	else if (object == DrawingObjectType::MODEL)
	{
		glm::mat4 model(1.0f);
		m_view = Default_Values::camera.getViewMatrix();
		shader.setVec3("lightPos", Default_Values::lightPos);
		shader.setMat4("model", model);
		shader.setMat4("view", m_view);
		shader.setMat4("projection", m_projection);
		// for shadows
		shader.setMat4("lightSpaceMatrix", m_lightProjectionMatrix);
	}
	else if (object == DrawingObjectType::FLOOR)
	{
		m_view = Default_Values::camera.getViewMatrix();
		glm::mat4 res = m_projection * m_view;
		glm::vec3 cameraWorldPos = Default_Values::camera.Position;


		shader.setMat4("gridViewProjection", res);
		shader.setVec3("cameraWorldPosition", cameraWorldPos);
	}
}

void fillPerspectiveMatrix(PerspectiveValues& pValues)
{
	pValues.fov = Default_Values::camera.fov;
	pValues.scr_height = Default_Values::SCR_HEIGHT;
	pValues.scr_width = Default_Values::SCR_WIDTH;
	pValues.zNear = Default_Values::zNear;
	pValues.zFar = Default_Values::zFar;
}