#ifndef CAMERA_H
#define CAMERA_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum class CameraMovement
{
	forward,
	backward,
	left,
	right,			
};



struct PerspectiveValues
{
	float fov{};
	int scr_width{};
	int scr_height{};
	float zNear{};
	float zFar{};
};

constexpr float g_yaw{ -90.0f };
constexpr float g_pitch{ 0.0f };
constexpr float g_sensitivity{ 0.1f };
constexpr float g_speed{ 2.5f };

constexpr float g_maxDistance{ 100.0f };
constexpr float g_minDistance{ 5.0f };

class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float Sensitivity;

	float fov{ 90.0f };


	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = g_yaw, float pitch = g_pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(g_speed), Sensitivity(g_sensitivity)
	{
		Position = pos;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCamVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(g_speed), Sensitivity(g_sensitivity)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCamVectors();
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;

		if (direction == CameraMovement::forward)
		{
			Position += Front * velocity;
		}
		if (direction == CameraMovement::backward)
		{
			Position -= Front * velocity;
		}
		if (direction == CameraMovement::left)
		{
			Position -= Right * velocity;
		}
		if (direction == CameraMovement::right)
		{
			Position += Right * velocity;
		}

	}

	void processMouse(float xOffset, float yOffset, GLboolean constraintPitch = true)
	{
		xOffset *= Sensitivity;
		yOffset *= Sensitivity;

		Yaw += xOffset;
		Pitch += yOffset;

		if (constraintPitch)
		{
			if (Pitch > 89.0f) { Pitch = 89.0f; }
			if (Pitch < -89.0f) { Pitch = -89.0f; }

		}
		updateCamVectors();
	}


private:
	void updateCamVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};


#endif 
