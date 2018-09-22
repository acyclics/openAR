#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "opengl.h"
#include "graphic.h"

#include <string>
#include <iostream>

/*  Graphic  */
Graphic::Graphic()
	: m_shader("default.vs", "default.fs")
{
	float vertices[32];
	vector<unsigned int> indices(6);
	gl::defaultRectangleVertices(vertices);
	gl::defaultRectangleIndices(indices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_indices = indices;
	m_shader.use();
}

Graphic::Graphic(std::string path)
	: m_shader("default.vs", "default.fs"), m_path(path)
{
	float vertices[32];
	vector<unsigned int> indices(6);
	gl::defaultRectangleVertices(vertices);
	gl::defaultRectangleIndices(indices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_indices = indices;
	m_shader.use();
}

Graphic::Graphic(std::string path, Shader shader)
	: m_shader(shader), m_path(path)
{
	float vertices[32];
	vector<unsigned int> indices(6);
	gl::defaultRectangleVertices(vertices);
	gl::defaultRectangleIndices(indices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_indices = indices;
	m_shader.use();
	// add check for no coords etc
}

Graphic::Graphic(std::string path, float coordinates[])
	: m_shader("default.vs", "default.fs"), m_path(path)
{
	vector<unsigned int> indices(6);
	gl::defaultRectangleIndices(indices);
	m_indices = indices;
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(std::string path, Shader shader, float coordinates[])
	: m_shader(shader), m_path(path)
{
	vector<unsigned int> indices(6);
	gl::defaultRectangleIndices(indices);
	m_indices = indices;
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(std::string path, float coordinates[], vector<unsigned int> indices)
	: m_shader("default.vs", "default.fs"), m_path(path), m_indices(indices)
{
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(std::string path, Shader shader, float coordinates[], vector<unsigned int> indices)
	: m_shader(shader), m_path(path), m_indices(indices)
{
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(float coordinates[])
	: m_shader("default.vs", "default.fs")
{
	vector<unsigned int> indices(6);
	gl::defaultRectangleIndices(indices);
	m_vertex = gl::floatToVertex2d(coordinates);
	m_indices = indices;
	m_shader.use();
}

Graphic::Graphic(float coordinates[], vector<unsigned int> indices)
	: m_shader("default.vs", "default.fs"), m_indices(indices)
{
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(vector<unsigned int> indices)
	: m_shader("default.vs", "default.fs"), m_indices(indices)
{
	float vertices[32];
	gl::defaultRectangleVertices(vertices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_shader.use();
}

Graphic::Graphic(Shader shader)
	: m_shader(shader)
{
	float vertices[32];
	vector<unsigned int> indices(6);
	gl::defaultRectangleVertices(vertices);
	gl::defaultRectangleIndices(indices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_indices = indices;
	m_shader.use();
}

Graphic::Graphic(Shader shader, float coordinates[], vector<unsigned int> indices)
	: m_shader(shader), m_indices(indices)
{
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(Shader shader, float coordinates[])
	: m_shader(shader)
{
	vector<unsigned int> indices(6);
	gl::defaultRectangleIndices(indices);
	m_indices = indices;
	m_vertex = gl::floatToVertex2d(coordinates);
	m_shader.use();
}

Graphic::Graphic(Shader shader, vector<unsigned int> indices)
	: m_shader(shader), m_indices(indices)
{
	float vertices[32];
	gl::defaultRectangleVertices(vertices);
	m_vertex = gl::floatToVertex2d(vertices);
	m_shader.use();
}

/* Graphic functions */
std::string Graphic::path()
{
	return m_path;
}

vector<Vertex2D>& Graphic::vertex()
{
	return m_vertex;
}

vector<Vertex2D>* Graphic::vertex_ptr()
{
	return &m_vertex;
}

vector<Vertex2D> Graphic::copy_vertex()
{
	return m_vertex;
}

vector<unsigned int>& Graphic::indices()
{
	return m_indices;
}

vector<unsigned int>* Graphic::indices_ptr()
{
	return &m_indices;
}

vector<unsigned int> Graphic::copy_indices()
{
	return m_indices;
}

Shader& Graphic::shader()
{
	return m_shader;
}

Shader* Graphic::shader_ptr()
{
	return &m_shader;
}

Shader Graphic::copy_shader()
{
	return m_shader;
}

Mesh2D& Graphic::mesh()
{
	return m_mesh;
}

Mesh2D* Graphic::mesh_ptr()
{
	return &m_mesh;
}

Mesh2D Graphic::copy_mesh()
{
	return m_mesh;
}

void Graphic::setPath(std::string path)
{
	m_path = path;
}

void Graphic::setVertex(float coordinates[])
{
	m_vertex = gl::floatToVertex2d(coordinates);
}

void Graphic::setVertex(vector<Vertex2D> vertex)
{
	m_vertex = vertex;
}

void Graphic::setIndices(vector<unsigned int> indices)
{
	m_indices = indices;
}

void Graphic::setShader(Shader shader)
{
	m_shader = shader;
}

void Graphic::setMesh(Mesh2D mesh)
{
	m_mesh = mesh;
}

// overload operators
Graphic& Graphic::operator=(Graphic& graphic)
{
	m_path = graphic.m_path;
	m_vertex = graphic.m_vertex;
	m_indices = graphic.m_indices;
	m_shader = graphic.m_shader;
	m_mesh = graphic.m_mesh;
}
/*  End of Graphic  */
