/*
graphicsmain.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "opengl.h"
#include "mesh2D.h"

#include <iostream>

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
// glfw: initialize and configure
// ------------------------------
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

// glfw window creation
// --------------------
GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Camera", NULL, NULL);
if (window == NULL)
{
std::cout << "Failed to create GLFW window" << std::endl;
glfwTerminate();
return -1;
}
glfwMakeContextCurrent(window);
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

// glad: load all OpenGL function pointers
// ---------------------------------------
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
std::cout << "Failed to initialize GLAD" << std::endl;
return -1;
}

// configure global opengl state
// -----------------------------
//glEnable(GL_DEPTH_TEST);

// build and compile shaders
// -------------------------
Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
ourShader.use();

Shader ourShader_camera("4.1.texture.vs", "4.1.texture.fs");
ourShader_camera.use();

float vertices[] = {
// positions          // colors           // texture coords
0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

cv::Mat frame;
cv::VideoCapture cam;
cam.open(0);
parseCamera(cam, frame);
editCameraVertices(vertices, frame, SCR_WIDTH, SCR_HEIGHT);

vector<unsigned int> indices = {
0, 1, 3, // first triangle
1, 2, 3  // second triangle
};

vector<Vertex2D> vertex2d = floatToVertex2d(vertices);

unsigned int texid(0);
Mesh2D cameraMesh(vertex2d, indices, ourShader_camera, texid);

// load models
// -----------
Model ourModel("nanosuit/nanosuit.obj");

// draw in wireframe
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// render loop
// -----------
while (!glfwWindowShouldClose(window))
{
// per-frame time logic
// --------------------
float currentFrame = glfwGetTime();
deltaTime = currentFrame - lastFrame;
lastFrame = currentFrame;

// input
// -----
processInput(window);

// render
// ------
ourShader_camera.use();
parseCamera(cam, frame);
cv::Mat frame_flipped;
cv::flip(frame, frame_flipped, 1);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_flipped.size().width, frame_flipped.size().height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)frame_flipped.data);
glGenerateMipmap(GL_TEXTURE_2D);

// clear
glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glClear(GL_COLOR_BUFFER_BIT);

cameraMesh.Draw(ourShader_camera, texid);

// enable depth
glEnable(GL_DEPTH_TEST);
glClear(GL_DEPTH_BUFFER_BIT);

// view/projection transformations
ourShader.use();

glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view = camera.GetViewMatrix();
ourShader.setMat4("projection", projection);
ourShader.setMat4("view", view);

// render the loaded model
glm::mat4 model;
model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f)); // translate it down so it's at the center of the scene
model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
ourShader.setMat4("model", model);
ourModel.Draw(ourShader);

// disable depth
glDisable(GL_DEPTH_TEST);

// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// -------------------------------------------------------------------------------
glfwSwapBuffers(window);
glfwPollEvents();
}

// glfw: terminate, clearing all previously allocated GLFW resources.
// ------------------------------------------------------------------
glfwTerminate();
return 0;
}



mesh2D.h

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture_unit_count.h"

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
/*  Mesh Data 
vector<Vertex2D> vertices;
vector<unsigned int> indices;
unsigned int VAO;

/*  Functions 
// constructor
Mesh2D(vector<Vertex2D> vertices, vector<unsigned int> indices, Shader shader, unsigned int texid)
{
	this->vertices = vertices;
	this->indices = indices;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh(shader, texid);
}

// render the mesh
void Draw(Shader shader, unsigned int texid)
{
	// bind texture
	glActiveTexture(GL_TEXTURE0 + 1);
	//glUniform1i(glGetUniformLocation(shader.ID, "camtexture1"), texid);
	shader.setInt("camtexture1", texid);

	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, texid);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

private:
	/*  Render data 
	unsigned int VBO, EBO;

	/*  Functions   
	// initializes all the buffer objects/arrays
	void setupMesh(Shader shader, unsigned int texid)
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
		glGenTextures(1, &texid);	// first parameter takes as input how many textures we want to generate, then stores it in texid
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//shader.setInt("texture1", 1);

		// increase texture unit by one
		++meshTextureUnit;
		mesh2D_tallyTextureUnit.push_back(meshTextureUnit);
	}
};




mesh.h

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture_unit_count.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
// position
glm::vec3 Position;
// normal
glm::vec3 Normal;
// texCoords
glm::vec2 TexCoords;
// tangent
glm::vec3 Tangent;
// bitangent
glm::vec3 Bitangent;
};

struct Texture {
unsigned int id;
string type;
string path;
};

class Mesh {
public:
/*  Mesh Data 
vector<Vertex> vertices;
vector<unsigned int> indices;
vector<Texture> textures;
unsigned int VAO;

/*  Functions 
// constructor
Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

// render the mesh
void Draw(Shader shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)

		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
												 //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i+1);
		shader.setInt((name + number).c_str(), i + 1);

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

private:
	/*  Render data 
	unsigned int VBO, EBO;

	/*  Functions  
	// initializes all the buffer objects/arrays
	void setupMesh()
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
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);

		// increase texture unit by one
		++meshTextureUnit;
		mesh_tallyTextureUnit.push_back(meshTextureUnit);
	}
};


*/