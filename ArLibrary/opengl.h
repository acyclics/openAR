#pragma once
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <iostream>
#include "mesh2D.h"

// opengl core functions

/* opengl functions */
namespace gl
{

	/* Native functions */
	vector<Vertex2D> floatToVertex2d(float vertices[]);

	void defaultRectangleVertices(float vertices[]);

	void defaultRectangleIndices(vector<unsigned int>& indices);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void processInput(GLFWwindow *window);

	GLFWwindow* createGlWindow(unsigned int width, unsigned int height, char name[]);

	void clearGraphics(float red = 0.05, float green = 0.05, float blue = 0.05, float alpha = 1.0);
}