#ifndef CAMERA_H
#define CAMERA_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FWD,
	BCK,
	LT,
	RT
};

const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.f;


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY,float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FWD)
		{
			Position += Front * velocity;
		}

		else if (direction == BCK)
		{
			Position -= Front * velocity;
		}

		else if (direction == LT)
		{
			Position -= Right * velocity;
		}

		else if (direction == RT)
		{
			Position += Right * velocity;
		}

	}


	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.f)
			{
				Pitch = 89.f;
			}
			else if (Pitch < -89.f)
			{
				Pitch = -89.f;
			}
		}

		updateCameraVectors();

	}

	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.f)
		{
			Zoom = 1.f;
		}
		else if (Zoom > 45.f)
		{
			Zoom = 45.f;
		}
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));//Front cant be parrallerl to WorldUp
		Up = glm::normalize(glm::cross(Right, Front));


	}



};

#endif