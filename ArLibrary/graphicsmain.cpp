#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "graphic.h"
#include "image.h"
#include "opencv2\opencv.hpp"
#include "camera.h"
#include "opengl.h"
#include "model.h"
#include "cmra.h"

#include <iostream>


using namespace cv;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
	GLFWwindow* window = gl::createGlWindow(SCR_WIDTH, SCR_HEIGHT, "Window");

	// camera
	float vertices[32];
	gl::defaultRectangleVertices(vertices);
	vector<unsigned int> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	cam camera1(vertices, indices, SCR_WIDTH, SCR_HEIGHT);

	// load models
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
	Model ourModel("nanosuit/nanosuit.obj");

	// CONTAINER
	float container_vertices[] = {
		// positions          // colors           // texture coords
		-0.2f,  -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.2f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.4f, -0.3f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.4f, -0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	vector<unsigned int> container_indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	Image box("boxes/container.jpg", container_vertices, container_indices);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		gl::clearGraphics();
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		gl::processInput(window);

		// camera
		camera1.draw();

		// view/projection transformations
		ourModel.changePerspective(ourShader, camera, SCR_WIDTH, SCR_HEIGHT);

		// render the loaded model
		glm::mat4 model;
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// box
		box.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}