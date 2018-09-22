#pragma once
#include "opengl.h"
#include "graphic.h"
#include "image.h"

namespace cmra
{
	void parseCamera(cv::VideoCapture& cam, cv::Mat& frame);

	void editCameraVertices(float vertices[], cv::Mat frame, float SCR_WIDTH, float SCR_HEIGHT);
}

class cam : Graphic
{
	// the order in which the initialization list initialize these variables is according to the order in which they are declared below
	int m_width;
	int m_height;
	cv::Mat m_frame;
	cv::VideoCapture m_cam;

public:
	cam(float vertices[], std::vector<unsigned int> indices, float SCR_WIDTH = 720, float SCR_HEIGHT = 480);
	cam(int device, float vertices[], std::vector<unsigned int> indices, float SCR_WIDTH = 720, float SCR_HEIGHT = 480);
	cam(string filename, float vertices[], std::vector<unsigned int> indices, float SCR_WIDTH = 720, float SCR_HEIGHT = 480);

	void draw();
};