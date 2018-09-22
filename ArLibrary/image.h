/*
Image is the way to implement an image into opengl window. Simply initiailize an image with coordinates etc,
then use the draw function to draw the image.

The function replaceWith can be used to replace the image each frame (so each interation of while loop). However,
opengl works by drawing the images out after the end of each while loop (because that is when we call buffer)
*/

#pragma once
#include <string>
#include "shader.h"
#include <vector>
#include "mesh2D.h"
#include "graphic.h"
#include "opencv2\core.hpp"

class Image : Graphic
{
	int m_width;
	int m_height;
	int m_nrchannels;
	unsigned char* m_data;

public:

	// constructors
	Image();
	Image(std::string path);
	Image(std::string path, Shader shader);
	Image(std::string path, Shader shader, float coordinates[]);
	Image(std::string path, Shader shader, float coordinates[], std::vector<unsigned int> indices);
	Image(std::string path, float coordinates[]);
	Image(std::string path, float coordinates[], std::vector<unsigned int> indices);
	Image(unsigned char* data);
	Image(unsigned char* data, float coordinates[]);
	Image(unsigned char* data, float coordinates[], std::vector<unsigned int> indices);
	Image(unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices);
	Image(unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels);
	Image(GLvoid * data);
	Image(GLvoid * data, Shader shader);
	Image(GLvoid * data, Shader shader, float coordinates[]);
	Image(GLvoid * data, Shader shader, float coordinates[], std::vector<unsigned int> indices);
	Image(GLvoid * data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels);
	Image(void* ptr, unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels);

	// functions
	/*
	If you want that each time we call draw to instantly draw the image without waiting for end of frame buffer,
	then add "glfwSwapBuffers(window);" to the end of the draw function
	*/
	void draw();
	void replaceWith(unsigned char* data);
	void replaceWith(std::string path);
	void replaceWith(GLvoid * data);
	void replaceWith(cv::Mat frame);
	void adjust(int width, int height, int channels);

	// overload operators
	Image& Image::operator=(Image& image);
	friend bool Image::operator==(const Image& lhs, const Image& rhs);
};