/*
mesh2D is a container designed to "house" images (2D images).
It also deals with the opengl functionality of drawing the object using Draw().
It uses the shader class to load the fragment and vertex shaders.
However, images should not be initialized through mesh2D. A separate class, "image", should be used which
abstracts away even more functionality.
*/

#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex2D {
	// position
	glm::vec3 Position;
	// color
	glm::vec3 Color;
	// texCoords
	glm::vec2 TexCoords;
};

class Mesh2D {
public:
	/*  Mesh Data  */
	vector<Vertex2D> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;

	/*  Functions  */
	// constructor
	Mesh2D(vector<Vertex2D> vertices, vector<unsigned int> indices, Shader shader)
	{
		this->vertices = vertices;
		this->indices = indices;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh(shader);
	}

	Mesh2D()
	{
		// must check for errors somehow
	}

	// render the mesh
	void Draw(Shader shader)
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0);
		shader.setInt("camtexture1", 0);

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

	// overload library operators
	Mesh2D& Mesh2D::operator=(const Mesh2D& mesh)
	{
		this->vertices = mesh.vertices;
		this->indices = mesh.indices;
		this->VAO = mesh.VAO;
		this->VBO = mesh.VBO;
		this->EBO = mesh.EBO;
		return *this;
	}

private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh(Shader shader)
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), &vertices[0], GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
		// vertex color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, Color));
		// vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, TexCoords));
		
		//glBindVertexArray(0);

		// bind appropriate textures
		unsigned int mesh2d_texid;
		glGenTextures(1, &mesh2d_texid);	// first parameter takes as input how many textures we want to generate, then stores it in texid
		glBindTexture(GL_TEXTURE_2D, mesh2d_texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
};