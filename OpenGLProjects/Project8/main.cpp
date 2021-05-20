

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shaders.h>
#include <Camera.h>
#include <Model.h>
#include <Controller.h>

#include <iostream>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.f, 0.f, 3.f));
float lastX = SCR_WIDTH / 2.f;
float lastY = SCR_HEIGHT / 2.f;
bool firstMouse = true;

float deltaTime = 0.f;
float lastFrame = 0.f;

int main()
{
	//init

	glfwInit();
	GLFWwindow* window = Controller::initGLFW(SCR_WIDTH, SCR_HEIGHT, "Project8");

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);

	});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		camera.ProcessMouseScroll(yoffset);
	});

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//-init

	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);

	int select = 1;
	Shaders shader("explode.vs", "explode.fs", "explode.gs");
	Shaders defshader("default.vs", "default.fs");
	Shaders normalShader("normal.vs", "normal.fs", "normal.gs");

	Model nanosuit("../../resources/objects/nanosuit/nanosuit.obj");
	

	while (!glfwWindowShouldClose(window))
	{


		auto currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Controller::processInput(window, camera, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			select = 1;


		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			select = 2;


		}



		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		
		if(select == 1)
		{
			shader.use();

			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			shader.setMat4("model", model);
		

			// add time component to geometry shader in the form of a uniform
			shader.setFloat("time", glfwGetTime());

			// draw model
			nanosuit.Draw(shader);
		}
		if (select == 2)
		{
			defshader.use();
			defshader.setMat4("projection", projection);
			defshader.setMat4("view", view);
			defshader.setMat4("model", model);

			// draw model as usual
			nanosuit.Draw(shader);

			// then draw model with normal visualizing geometry shader
			normalShader.use();
			normalShader.setMat4("projection", projection);
			normalShader.setMat4("view", view);
			normalShader.setMat4("model", model);

			nanosuit.Draw(normalShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	

	glfwTerminate();
	return 0;
}




