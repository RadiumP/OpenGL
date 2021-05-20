#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;
class Controller
{
public:
	

	static void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
	static GLFWwindow* initGLFW(unsigned int width, unsigned int height, const char* title);


};

