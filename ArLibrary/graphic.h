#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh2D.h"

class Graphic
{
	std::string m_path;
	vector<Vertex2D> m_vertex;
	vector<unsigned int> m_indices;

public:

	Shader m_shader;
	Mesh2D m_mesh;

	// constructors
	Graphic();
	Graphic(std::string path);
	Graphic(std::string path, Shader shader);
	Graphic(std::string path, Shader shader, float coordinates[]);
	Graphic(std::string path, Shader shader, float coordinates[], vector<unsigned int> indices);
	Graphic(std::string path, float coordinates[]);
	Graphic(std::string path, float coordinates[], vector<unsigned int> indices);
	Graphic(float coordinates[]);
	Graphic(float coordinates[], vector<unsigned int> indices);
	Graphic(vector<unsigned int> indices);
	Graphic(Shader shader);
	Graphic(Shader shader, float coordinates[], vector<unsigned int> indices);
	Graphic(Shader shader, float coordinates[]);
	Graphic(Shader shader, vector<unsigned int> indices);

	// functions
	std::string path();
	vector<Vertex2D>& vertex();
	vector<Vertex2D>* vertex_ptr();
	vector<Vertex2D> copy_vertex();
	vector<unsigned int>& indices();
	vector<unsigned int>* indices_ptr();
	vector<unsigned int> copy_indices();
	Shader& shader();
	Shader* shader_ptr();
	Shader copy_shader();
	Mesh2D& mesh();
	Mesh2D* mesh_ptr();
	Mesh2D copy_mesh();
	void setPath(std::string path);
	void setVertex(float coordinates[]);
	void setVertex(vector<Vertex2D> vertex);
	void setIndices(vector<unsigned int> indices);
	void setShader(Shader shader);
	void setMesh(Mesh2D mesh);

	// overload operators
	Graphic& Graphic::operator=(Graphic& graphic);
	friend bool Graphic::operator==(const Graphic& lhs, const Graphic& rhs);
};
