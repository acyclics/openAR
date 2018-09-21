#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cmra.h"

// namespace here
void cmra::parseCamera(cv::VideoCapture& cam, cv::Mat& frame)
{
	// opencv create camera
	using namespace cv;
	Mat camframe, toberotatedframe;
	cam >> camframe;
	cvtColor(camframe, toberotatedframe, CV_BGR2RGB);
	Mat rotatemat = getRotationMatrix2D(Point2f(toberotatedframe.cols / 2, toberotatedframe.rows / 2), 180, 1);
	warpAffine(toberotatedframe, frame, rotatemat, toberotatedframe.size());
}

void cmra::editCameraVertices(float vertices[], cv::Mat frame, float SCR_WIDTH, float SCR_HEIGHT)
{
	float xcoord;
	float ycoord;

	xcoord = ((float)frame.size().width / (float)SCR_WIDTH);
	ycoord = ((float)frame.size().height / (float)SCR_HEIGHT);
	vertices[0] = xcoord;
	vertices[1] = ycoord;
	vertices[8] = xcoord;
	vertices[9] = -ycoord;
	vertices[16] = -xcoord;
	vertices[17] = -ycoord;
	vertices[24] = -xcoord;
	vertices[25] = ycoord;
}

// class here
cam::cam(float vertices[], std::vector<unsigned int> indices, float SCR_WIDTH, float SCR_HEIGHT)
	: Graphic(Shader("camera.vs", "camera.fs"), indices), m_width(SCR_WIDTH), m_height(SCR_HEIGHT)
{
	m_cam.open(0);
	cmra::parseCamera(m_cam, m_frame);
	cmra::editCameraVertices(vertices, m_frame, m_width, m_height);
	this->setVertex(vertices);
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

void cam::draw()
{
	cmra::parseCamera(m_cam, m_frame);
	//cv::Mat frame_flipped;
	//cv::flip(m_frame, frame_flipped, 1);
	// clear
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_shader.use();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frame.size().width, m_frame.size().height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_mesh.Draw(m_shader);
}
