#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <iostream>
#include "mesh2D.h"
#include <vector>
#include "exception.h"
#include "opengl.h"

// opengl core functions

/* Native functions */

vector<Vertex2D> gl::floatToVertex2d(float vertices[])
{
	vector<Vertex2D> vertex(4);
	vertex[0].Position = glm::vec3(vertices[0], vertices[1], vertices[2]);
	vertex[0].Color = glm::vec3(vertices[3], vertices[4], vertices[5]);
	vertex[0].TexCoords = glm::vec2(vertices[6], vertices[7]);
	vertex[1].Position = glm::vec3(vertices[8], vertices[9], vertices[10]);
	vertex[1].Color = glm::vec3(vertices[11], vertices[12], vertices[13]);
	vertex[1].TexCoords = glm::vec2(vertices[14], vertices[15]);
	vertex[2].Position = glm::vec3(vertices[16], vertices[17], vertices[18]);
	vertex[2].Color = glm::vec3(vertices[19], vertices[20], vertices[21]);
	vertex[2].TexCoords = glm::vec2(vertices[22], vertices[23]);
	vertex[3].Position = glm::vec3(vertices[24], vertices[25], vertices[26]);
	vertex[3].Color = glm::vec3(vertices[27], vertices[28], vertices[29]);
	vertex[3].TexCoords = glm::vec2(vertices[30], vertices[31]);

	return vertex;
}

void gl::defaultRectangleVertices(float vertices[])
{
	float defaultvertice[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	
	for (int i(0); i < 32; ++i)
		vertices[i] = defaultvertice[i];
}

void gl::defaultRectangleIndices(vector<unsigned int>& indices)
{
	vector<unsigned int> defaultindice = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	for (int i(0); i < 6; ++i)
		indices[i] = defaultindice[i];
}

/* Opengl functions */
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void gl::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void gl::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// create Gl window
GLFWwindow* gl::createGlWindow(unsigned int width, unsigned int height, char name[])
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		throw Exception::Errors::GLWINDOW;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, gl::framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw Exception::Errors::GLGLAD;
	}

	return window;
}

void gl::clearGraphics(float red, float green, float blue, float alpha)
{
	// 0.05, 0.05, 0.05, 1.0 by default
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}