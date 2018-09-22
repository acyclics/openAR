#include "image.h"
#include "opengl.h"
#include "stb_image.h"
#include "opencv2\core.hpp"

/*  Image */
Image::Image()
	: Graphic(), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path)
	: Graphic(path), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path, Shader shader)
	: Graphic(path, shader), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path, float coordinates[])
	: Graphic(path, coordinates)
{
	m_data = stbi_load(this->path().c_str(), &m_width, &m_height, &m_nrchannels, 0);
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path, Shader shader, float coordinates[])
	: Graphic(path, shader, coordinates)
{
	m_data = stbi_load(this->path().c_str(), &m_width, &m_height, &m_nrchannels, 0);
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path, float coordinates[], vector<unsigned int> indices)
	: Graphic(path, coordinates, indices)
{
	m_data = stbi_load(this->path().c_str(), &m_width, &m_height, &m_nrchannels, 0);
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(std::string path, Shader shader, float coordinates[], vector<unsigned int> indices)
	: Graphic(path, shader, coordinates, indices)
{
	m_data = stbi_load(this->path().c_str(), &m_width, &m_height, &m_nrchannels, 0);
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(unsigned char* data)
	: Graphic(), m_data(data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(unsigned char* data, float coordinates[])
	: Graphic(coordinates), m_data(data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(unsigned char* data, float coordinates[], std::vector<unsigned int> indices)
	: Graphic(coordinates, indices), m_data(data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices)
	: Graphic(shader, coordinates, indices), m_data(data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels)
	: Graphic(shader, coordinates, indices), m_data(data), m_width(width), m_height(height), m_nrchannels(channels)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(GLvoid * data)
	: Graphic(), m_data((unsigned char*)data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(GLvoid * data, Shader shader)
	: Graphic(shader), m_data((unsigned char*)data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(GLvoid * data, Shader shader, float coordinates[])
	: Graphic(shader, coordinates), m_data((unsigned char*)data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(GLvoid * data, Shader shader, float coordinates[], std::vector<unsigned int> indices)
	: Graphic(shader, coordinates, indices), m_data((unsigned char*)data), m_width(720), m_height(480), m_nrchannels(3)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(GLvoid * data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels)
	: Graphic(shader, coordinates, indices), m_data((unsigned char*)data), m_width(width), m_height(height), m_nrchannels(channels)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

Image::Image(void* ptr, unsigned char* data, Shader shader, float coordinates[], std::vector<unsigned int> indices, int width, int height, int channels)
	: Graphic(shader, coordinates, indices), m_data(data), m_width(width), m_height(height), m_nrchannels(channels)
{
	Mesh2D tempMesh(this->vertex(), this->indices(), m_shader);
	m_mesh = tempMesh;
}

void Image::draw()
{
	// clear
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_shader.use();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_mesh.Draw(m_shader);
}

void Image::replaceWith(unsigned char* data)
{
	m_data = data;
}

void Image::replaceWith(std::string path)
{
	this->setPath(path);
	m_data = stbi_load(this->path().c_str(), &m_width, &m_height, &m_nrchannels, 0);
}

void Image::replaceWith(GLvoid * data)
{
	m_data = (unsigned char*)data;
}

void Image::replaceWith(cv::Mat frame)
{
	m_data = frame.data;
	m_width = frame.size().width;
	m_height = frame.size().height;
	m_nrchannels = frame.channels();
}

void Image::adjust(int width, int height, int channels)
{
	m_width = width;
	m_height = height;
	m_nrchannels = channels;
}

Image& Image::operator=(Image& image)
{
	// image here
	this->m_width = image.m_width;
	this->m_height = image.m_height;
	this->m_nrchannels = image.m_nrchannels;
	this->m_data = image.m_data;

	// graphic here
	this->setPath(image.path());
	this->setVertex(image.vertex());
	this->setIndices(image.indices());
	this->setShader(image.shader());
	this->setMesh(image.mesh());
	return *this;
}

bool operator==(const Image& lhs, const Image& rhs)
{
	if (lhs.m_width == rhs.m_width && lhs.m_height == rhs.m_height && lhs.m_nrchannels == rhs.m_nrchannels
		&& lhs.m_data == rhs.m_data && static_cast<Graphic>(lhs) == static_cast<Graphic>(rhs))
		return true;
	else
		return false;
}
/*  End of Image  */
