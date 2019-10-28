//Comp 371 assignment thomas groves
#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include <FreeImageIO.h>
#include<vector>
#include<math.h>
#include<GL/GLU.h>



using namespace glm;
using namespace std;


const char* getVertexShaderSource()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform vec4 inColor;"
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec4 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = inColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}


const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec4 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

const char* getTexturedVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec2 aUV;"
		"layout (location = 2) in vec3 aNorm;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"out vec2 vertexUV;"
		"out  vec3 normal;"
		"out vec3 FragPos;"
		""
		"void main()"
		"{"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   vertexUV = aUV;"
		"	normal = mat3(transpose(inverse(modelViewProjection))) * aNorm;"
		"	FragPos = vec3(worldMatrix*vec4(aPos, 1.0));"
		"}";
}

const char* getTexturedFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"uniform sampler2D textureSampler;"
		""
		"out vec4 FragColor;"
		"in vec3 FragPos;"
		"in vec3 normal;"
		"uniform vec3 objectColor = vec3(1.0);"
		"uniform vec3 lightColor;"
		"uniform vec3 lightPos;"
		"uniform bool textured;"
		"struct Material{"
		"	vec3 ambient;"
		"	vec3 diffuse;"
		"	vec3 specular;"
		"	float shininess;"
		"};"
		"uniform Material material;"
		"uniform vec3 viewPos;"
		"struct Light{"
		"	vec3 ambient;"
		"	vec3 diffuse;"
		"	vec3 specular;"
		"	vec3 position;"
		"};"
		"uniform Light light;"
		"void main()"
		"{"
		"   vec4 textureColor = texture( textureSampler, vertexUV );"
		//ambient
		"	float ambientStrength = 0.1;"
		"	vec3 ambient = material.ambient * light.ambient;"
		//diffuse
		"	vec3 norm = normalize(normal);"
		"	vec3 lightDir = normalize(lightPos - FragPos);"
		"	float diff = max(dot(norm, lightDir), 0.0);"
		"	vec3 diffuse = (diff * material.diffuse) * light.diffuse;"
		//specular
		"	float specularStrength = 0.5;"
		"	vec3 viewDir = normalize(viewPos - FragPos);"
		"	vec3 reflectDir = reflect(-lightDir, norm);"
		"	float spec = pow(max(dot(viewDir,reflectDir), 0.0),32);"
		"	vec3 specular = ( spec *material.specular)* light.specular;"
		"	vec3 result;"
		"	if(textured)"
		"	{"
		"		result = (ambient + diffuse + specular)  * textureColor.xyz;"
		"	}else{"
		"		result = (ambient + diffuse + specular)  * objectColor;"
		"	}"
		"	FragColor = vec4(result, 1.0);"
		"}";
}


GLuint color;
int colorShaderProgram;
int texturedShaderProgram;
int lightingShaderProgram;
GLuint brickTextureID;
GLuint cementTextureID;
GLuint grassTextureID;
GLuint tireTextureID;
bool textured;
//colors
GLfloat red[] = { 1, 0, 0, 1 };
GLfloat green[] = { 0, 1, 0, 1 };
GLfloat blue[] = { 0, 0, 1, 1 };
GLfloat yellow[] = { 1, 1, 0, 1 };

//vertex arrays
int cylinderVBO;

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	color = glGetUniformLocation(shaderProgram, "inColor");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

struct TexturedColoredVertex
{
	TexturedColoredVertex(vec3 _position, vec2 _uv, vec3 _normal)
		: position(_position), uv(_uv) , normal(_normal) {}

	vec3 position;
	vec2 uv;
	vec3 normal;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position, UV, normal                           
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //left - 
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
																	 
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec2(1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec2(1.0f, 1.0f),  vec3(0.0f, 0.0f, -1.0f)), // far 
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec2(1.0f, 1.0f),  vec3(0.0f, 0.0f, -1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec2(1.0f, 0.0f),  vec3(0.0f, 0.0f, -1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec2(1.0f, 1.0f),  vec3(0.0f, -1.0f, 0.0f)), // bottom
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec2(1.0f, 0.0f),  vec3(0.0f, -1.0f, 0.0f)),
																	
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec2(1.0f, 1.0f),  vec3(0.0f, -1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)), // near 
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec2(1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)),
																	
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 1.0f),  vec3(0.0f, 0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec2(1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 1.0f),  vec3(1.0f, 0.0f, 0.0f)), // right
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f),  vec3(1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec2(1.0f, 0.0f),  vec3(1.0f, 0.0f, 0.0f)),
																	 
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec2(0.0f, 0.0f),  vec3(1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 1.0f),  vec3(1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec2(0.0f, 1.0f),  vec3(1.0f, 0.0f, 0.0f)),
																	  
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec2(10.0f, 10.0f), vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec2(10.0f, 0.0f),  vec3(0.0f, 1.0f, 0.0f)), // top changeing to UV coords of ten means we have our image repeat 10 times each axis
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec2(0.0f, 0.0f),  vec3(0.0f, 1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec2(10.0f, 10.0f), vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec2(0.0f, 0.0f),  vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 10.0f),  vec3(0.0f, 1.0f, 0.0f))
		};															  

const TexturedColoredVertex texturedCylinderVertexArray[] = {
	TexturedColoredVertex(vec3(0.0f, 0.5f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //1
	TexturedColoredVertex(vec3(0.35355f, 0.35355f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.0f, 0.5f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.35355f, 0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //1
	TexturedColoredVertex(vec3(0.35355f, 0.35355f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.0f, 0.5f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.35355f, 0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //2
	TexturedColoredVertex(vec3(0.35355f, 0.35355f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.0f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.0f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //2
	TexturedColoredVertex(vec3(0.35355f, 0.35355f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.0f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.0f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //3
	TexturedColoredVertex(vec3(0.5f, 0.0f, -0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.35355f, -0.35355f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.35355f, -0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //3
	TexturedColoredVertex(vec3(0.5f, 0.0f, -0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.35355f, -0.35355f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.35355f, -0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //4
	TexturedColoredVertex(vec3(0.35355f, -0.35355f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.0f, -0.5f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.35355f, -0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //4
	TexturedColoredVertex(vec3(0.0f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.0f, -0.5f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	
	TexturedColoredVertex(vec3(0.0f, -0.5f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //5
	TexturedColoredVertex(vec3(0.0f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //5
	TexturedColoredVertex(vec3(0.0f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //6
	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, -0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.0f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.5f, 0.0f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //6
	TexturedColoredVertex(vec3(-0.35355f,-0.35355f, -0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.0f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.5f, 0.0f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //7
	TexturedColoredVertex(vec3(-0.5f, 0.0f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //7
	TexturedColoredVertex(vec3(-0.5f, 0.0f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, 0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //8
	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, -0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.0f, 0.5f, 0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.0f, 0.5f, -0.5f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)), //8
	TexturedColoredVertex(vec3(0.0f, 0.5f, 0.5f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.35355f, 0.35355f, -0.5f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

};


int createCylinderBufferObject() {
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCylinderVertexArray), texturedCylinderVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);




	glVertexAttribPointer(1,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches normal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)((sizeof(vec3)) + (sizeof(vec2)))
	);
	glEnableVertexAttribArray(2);

	return vertexArrayObject;
}

int CreateLineBufferObject() {
	vec3 line[] = {
		vec3(-0.5, 0, 0), vec3(1.0f, 1.0f, 0.0f),
		vec3(0.5, 0, 0), vec3(1.0f, 1.0f, 0.0f)
	};
	// Create a vertex array
	GLuint lineArrayObject;
	glGenVertexArrays(1, &lineArrayObject);
	glBindVertexArray(lineArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint lineBufferObject;
	glGenBuffers(1, &lineBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lineBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0
	);
	glEnableVertexAttribArray(0);


	return lineArrayObject;
}

int createTexturedCubeVertexBufferObject()
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);




	glVertexAttribPointer(1,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches normal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)((sizeof(vec3)) + (sizeof(vec2)))      
	);
	glEnableVertexAttribArray(2);

	return vertexArrayObject;
}


int CreateLightVertexBufferObject() {
	vec3 cubeModel[] = {  // position, normal                           
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f), //left - 
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f),

		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),

		vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, 0.0f, 1.0f),// far - 
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f,-0.5f),  vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f,-0.5f, 0.5f),  vec3(-1.0f, 0.0f, 0.0f), // bottom - 
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f),  vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.5f,-0.5f, 0.5f),  vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),

		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // near - 
		vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f),  vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f),  vec3(1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f),  vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f, -1.0f, 0.0f),// right - 
		vec3(0.5f,-0.5f,-0.5f),  vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, -1.0f, 0.0f),


		vec3(0.5f,-0.5f,-0.5f),  vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f),  vec3(0.0f, -1.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),  // top - 
		vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f)
	};

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	GLuint lightBufferObject;
	glGenBuffers(1, &lightBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lightBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeModel), cubeModel, GL_STATIC_DRAW);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	return lightVAO;
}

int createShaddowFBO() {
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return depthMapFBO;
}

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setColorUniform(int shaderProgram, GLfloat setColor[]) {
	glUseProgram(shaderProgram);
	glUniform4fv(color, 1, setColor);
}

void setObjectColor(int shaderProgram, GLfloat setColor[]) {
	glUseProgram(shaderProgram);
	GLuint objectColor = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3fv(objectColor, 1, setColor);
}
void setLighttColor(int shaderProgram, GLfloat setColor[]) {
	glUseProgram(shaderProgram);
	GLuint lightColor = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3fv(lightColor, 1, setColor);
}

void setLightPos(int shaderProgram, GLfloat position[]) {
	glUseProgram(shaderProgram);
	GLuint lightPos = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightPos, 1, position);
}

void setViewPos(int shaderProgram, GLfloat position []) {
	glUseProgram(shaderProgram);
	GLuint viewPos = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3fv(viewPos, 1, position);
}

void setVec3(int shaderProgram, char* loc, vec3 value) {
	glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniform3fv(pos, 1, new float[3]{ value.x, value.y, value.z });
}

void setFloat(int shaderProgram, char* loc, float value) {
	glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniform1f(pos, value);
}

void setInt(int shaderProgram, char* loc, int value) {
	glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniform1i(pos, value);
}

void setMat4(int shaderProgram, char* loc, mat4 value) {
	glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniformMatrix4fv(pos, 1, GL_FALSE, &value[0][0]);
}





int loadTexture(char* imagepath)
{
	// Load image using the Free Image library
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
	FIBITMAP* image = FreeImage_Load(format, imagepath);
	FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

	// Get an available texture index from OpenGL
	GLuint texture = 0;
	glGenTextures(1, &texture);
	assert(texture != 0);

	// Set OpenGL filtering properties (bi-linear interpolation)
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Retrieve width and hight
	int width = FreeImage_GetWidth(image32bits);
	int height = FreeImage_GetHeight(image32bits);

	// This will upload the texture to the GPU memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

	// Free images
	FreeImage_Unload(image);
	FreeImage_Unload(image32bits);

	return texture;
}

float wheelAngle = 0.f;
float carPos[3] = { 0.0f, 1.5f, 0.0f };
float carUniformScaler = 1.f;
float carYRotation = 90.0f;
void DrawCar(int shaderProgram, float deltaTime, int drawMode) {

	float carScale[3] = { 1 * carUniformScaler, 1 * carUniformScaler, 1 * carUniformScaler };
	glPointSize(5.f);
	glLineWidth(5.f);
	//main body
	mat4 mainBodyWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	mainBodyWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	mainBodyWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	mainBodyWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * 3.0f, carScale[1], carScale[2] * 7.0f));
	setWorldMatrix(shaderProgram, mainBodyWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 36);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 36);
	}
	//top
	mat4 topCarWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	topCarWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1] + (1 * carUniformScaler), carPos[2]));
	topCarWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	topCarWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * 3.0f, carScale[1] * 1.5f, carScale[2] * 4.0f));
	setWorldMatrix(shaderProgram, topCarWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 36);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 36);
	}

	//roof 
	mat4 roofCarWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	roofCarWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1] + ((1 * carUniformScaler) + 1 * carUniformScaler), carPos[2]));
	roofCarWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	roofCarWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * 3.0f, carScale[1] * 0.5f, carScale[2] * 2.0f));
	setWorldMatrix(shaderProgram, roofCarWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 36);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 36);
	}

	//Wheels
	//wheelAngle += 180.f * deltaTime;
	glBindTexture(GL_TEXTURE_2D, tireTextureID);
	//front left
	float wheelScale = 0.8f;
	glBindVertexArray(cylinderVBO);
	mat4 wheelFLWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelFLWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelFLWorldMatrix *= translate(mat4(1.0f), vec3(1.75* carUniformScaler, -0.5f* carUniformScaler, 2.5* carUniformScaler));
	wheelFLWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * wheelScale, carScale[1] * wheelScale, carScale[2] * wheelScale));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	setWorldMatrix(shaderProgram, wheelFLWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 48);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 48);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 48);
	}


	//front right

	mat4 wheelFRWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelFRWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelFRWorldMatrix *= translate(mat4(1.0f), vec3(-1.75* carUniformScaler, -0.5f* carUniformScaler, 2.5* carUniformScaler));
	wheelFRWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * wheelScale, carScale[1] * wheelScale, carScale[2] * wheelScale));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	setWorldMatrix(shaderProgram, wheelFRWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 48);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 48);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 48);
	}
	//back left
	mat4 wheelBLWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelBLWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelBLWorldMatrix *= translate(mat4(1.0f), vec3(1.75 * carUniformScaler, -0.5f* carUniformScaler, -2.5* carUniformScaler));
	wheelBLWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * wheelScale, carScale[1] * wheelScale, carScale[2] * wheelScale));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	setWorldMatrix(shaderProgram, wheelBLWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 48);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 48);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 48);
	}

	//back right
	mat4 wheelBRWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelBRWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelBRWorldMatrix *= translate(mat4(1.0f), vec3(-1.75* carUniformScaler, -0.5f* carUniformScaler, -2.5* carUniformScaler));
	wheelBRWorldMatrix *= scale(mat4(1.0f), vec3(carScale[0] * wheelScale, carScale[1] * wheelScale, carScale[2] * wheelScale));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	setWorldMatrix(shaderProgram, wheelBRWorldMatrix);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 48);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 48);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 48);
	}


}

/*Draws the axis around the origin*/
void DrawAxis(int shaderProgram) {
	glLineWidth(20);
	mat4 lineWorldMatrixX = translate(mat4(1.0f), vec3(0, -2, 0)) * rotate(mat4(1.0f), radians(22.5f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(5));
	setWorldMatrix(shaderProgram, lineWorldMatrixX);
	setColorUniform(shaderProgram, red);
	glDrawArrays(GL_LINES, 0, 2);
	mat4 lineWorldMatrixY =  translate(mat4(1.0f), vec3(1.5, 3.25 - 2, 0))* rotate(mat4(1.0f), radians(22.5f), vec3(0.0f, 0.0f, 1.0f))* rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 0.0f, 1.0f))*scale(mat4(1.0f), vec3(5));
	setWorldMatrix(shaderProgram, lineWorldMatrixY);
	setColorUniform(shaderProgram, green);
	glDrawArrays(GL_LINES, 0, 2);
	mat4 lineWorldMatrixZ = translate(mat4(1.0f), vec3(-4, -1,1.55))* rotate(mat4(1.0f), radians(90.f - 22.5f), vec3(0.0f, 1.0f, 0.0f))*scale(mat4(1.0f), vec3(5))* rotate(mat4(1.0f), radians(22.5f), vec3(1.0f, 0.0f, 0.0f));
	setWorldMatrix(shaderProgram, lineWorldMatrixZ);
	setColorUniform(shaderProgram, blue);
	glDrawArrays(GL_LINES, 0, 2);
}

void DrawLamp(int shaderProgram) {
	mat4 lampWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	lampWorldMatrix *= translate(mat4(1.0f), vec3(0, 2.5, 0));
	lampWorldMatrix *= scale(mat4(1.0f), vec3(5.0f, 5.0f, 5.0f));
	setWorldMatrix(shaderProgram, lampWorldMatrix);
	setObjectColor(shaderProgram, new GLfloat[3]{ 1,0,0 });
	setLighttColor(shaderProgram, new GLfloat[3]{ 1,1,1 });
	glDrawArrays(GL_TRIANGLES, 0, 36);
}





int main(int argc, char*argv[])
{
	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);


	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}


	// Load Textures
#if defined(PLATFORM_OSX)
	GLuint brickTextureID = loadTexture("Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("Textures/cement.jpg");
	GLuint grassTextureID = loadTexture("Textures/grass.jpg");
	GLuint metalTextureID = loadTexture("Textures/metal.jpg");
	tireTextureID = loadTexture("Textures/tire.jpg");
#else
	GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
	GLuint grassTextureID = loadTexture("../Assets/Textures/grass.jpg");
	GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
	tireTextureID = loadTexture("../Assets/Textures/tire.jpg");
#endif

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Compile and link shaders here ...
	texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());

	//for shader uniforms
	color = glGetUniformLocation(colorShaderProgram, "inColor");

	// Camera parameters for view transform
	vec3 cameraPosition(0.6f, 1.0f, 10.0f);
	vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

	vec3 carPosition(0.f, 0.f, 0.f);
	vec3 carLookAt(0.0f, 1.0f, 0.0f);
	vec3 carHorizontal(0.0f, 0.0f,1.0f);


	// Other camera parameters
	float cameraSpeed = 1.0f;
	float cameraFastSpeed = 2 * cameraSpeed;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable


	// Spinning cube at camera position
	float spinningCubeAngle = 0.0f;

	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(lightingShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(lightingShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	// Define and upload geometry to the GPU here ...
	int lineVBO = CreateLineBufferObject();
	int lightVBO = CreateLightVertexBufferObject();
	int texturedCubeVBO = createTexturedCubeVertexBufferObject();
	cylinderVBO = createCylinderBufferObject();
	
	//framebuffer
	int shaddowFBO = createShaddowFBO();


	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	int lastSpaceState = GLFW_RELEASE;
	int lastAState = GLFW_RELEASE;
	int lastDState = GLFW_RELEASE;
	int lastWState = GLFW_RELEASE;
	int lastSState = GLFW_RELEASE;
	int lastQState = GLFW_RELEASE;
	int lastEState = GLFW_RELEASE;
	int lastXState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once
	// Enable Backface culling
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	glBindBuffer(GL_ARRAY_BUFFER, texturedCubeVBO);

	int drawMode = 0;
	int gridSize = 100;

	textured = true;

	vec3 lightPos(0, 3, -16);
	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;


		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (!textured) {
			setInt(texturedShaderProgram, "textured", 0);
		}
		else {

			setInt(texturedShaderProgram, "textured", 1);
		}
		// Draw textured geometry
		glUseProgram(texturedShaderProgram);
		setViewPos(texturedShaderProgram, new GLfloat[3]{ cameraPosition.x, cameraPosition.y , cameraPosition.z });

		setLightPos(texturedShaderProgram, new float[3]{ lightPos.x, lightPos.y, lightPos.z });
		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
		glBindTexture(GL_TEXTURE_2D, grassTextureID);
		glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0
		
		
		glBindVertexArray(texturedCubeVBO);
		mat4 lightWorldMatrix = translate(mat4(1.0f), vec3(lightPos.x, lightPos.y, lightPos.z));
		setWorldMatrix(texturedShaderProgram, lightWorldMatrix);
		setLighttColor(texturedShaderProgram, new float[3]{ 1,1,1});
		setVec3(texturedShaderProgram, "viewPos", cameraPosition);
		setVec3(texturedShaderProgram, "light.ambient", vec3(0.2f, 0.2f, 0.2f));
		setVec3(texturedShaderProgram, "light.diffuse", vec3(0.8f, 0.8f, 0.8f));
		setVec3(texturedShaderProgram, "light.specular", vec3(1.0f, 1.0f, 1.0f));
		setVec3(texturedShaderProgram, "objectColor", vec3(0.0f, 1.0f, 0.0f));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Draw ground
		mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(gridSize, 0.02f, gridSize));
		setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
		setVec3(texturedShaderProgram, "material.ambient", vec3(0.6f, 0.6f, 0.6f));
		setVec3(texturedShaderProgram, "material.diffuse", vec3(0.3f, 0.3f, 0.3f));
		setVec3(texturedShaderProgram, "material.specular", vec3(0.4f, 0.4f, 0.4f));
		setFloat(texturedShaderProgram, "material.shininess", GLfloat(8.f));
		
		glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


		setVec3(texturedShaderProgram, "material.ambient", vec3(0.19225f, 0.19225f, 0.19225f));
		setVec3(texturedShaderProgram, "material.diffuse", vec3(0.50754f, 0.50754f, 0.50754f));
		setVec3(texturedShaderProgram, "material.specular", vec3(0.508273f, 0.508273f, 0.508273f));
		setFloat(texturedShaderProgram, "material.shininess", GLfloat(89.6f));
		glBindTexture(GL_TEXTURE_2D, metalTextureID);

		carPos[0] = carPosition.x;
		carPos[2] = carPosition.z;
		DrawCar(texturedShaderProgram, dt, drawMode);

		
		// Draw colored geometry/*7

		glUseProgram(colorShaderProgram);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		//Draw Axis
		DrawAxis(colorShaderProgram);


		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float carSpeed = 5.f;
		float rotSpeed = 180;

		float carTheta = radians(carYRotation);
		carLookAt = vec3(-cosf(0)*sinf(carTheta), sin(0), -cosf(0) * cosf(carTheta)); //x and z flipped and then z inveresed because our car is drawn facing -x axis
		//CONTROLLS
		if (!glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move car to the left
			{

				carPosition += carSpeed * carHorizontal * dt;
				
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move car to the right
			{
				carPosition -= carSpeed * carHorizontal * dt;
			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move car up
			{
				carPosition -= carSpeed * carLookAt* dt;
				wheelAngle += 180.f * dt;
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move car down
			{
				carPosition += carSpeed * carLookAt* dt;
				wheelAngle -= 180.f * dt;
			}
			if (lastQState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // rotate car Left 5 degrees
			{
				carYRotation += 30 * dt;
			}

			if (lastEState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Rotate Car Right 5 degrees
			{
				carYRotation -= 30 * dt;
			}
		}
		else {
			if (lastAState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // rotate car Left 5 degrees
			{
				carPosition += vec3(0.5) * carHorizontal;
			}

			if (lastDState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Rotate Car Right 5 degrees
			{
				carPosition -= vec3(0.5) * carHorizontal;
			}

			if (lastWState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // rotate car Left 5 degrees
			{
				carPosition += vec3(0.5) * carLookAt;
			}

			if (lastSState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Rotate Car Right 5 degrees
			{
				carPosition -= vec3(0.5) * carLookAt;
			}

			if (lastQState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // rotate car Left 5 degrees
			{
				carYRotation += 5;
			}

			if (lastEState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Rotate Car Right 5 degrees
			{
				carYRotation -= 5;
			}
			//ensure only called once upon press
			lastAState = glfwGetKey(window, GLFW_KEY_A);
			lastDState = glfwGetKey(window, GLFW_KEY_D);
			lastWState = glfwGetKey(window, GLFW_KEY_W);
			lastSState = glfwGetKey(window, GLFW_KEY_S);

			lastQState = glfwGetKey(window, GLFW_KEY_Q);
			lastEState = glfwGetKey(window, GLFW_KEY_E);
		}


		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Scale Car Up
		{
			carUniformScaler += 0.2 * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Scale Car Down
		{
			carUniformScaler -= 0.2 * dt;
		}




		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // Draw the car as a point cloud
		{
			drawMode = 1;
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Draw car as lines
		{
			drawMode = 2;
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // Draw Car as triangle
		{
			drawMode = 0;
		}


		if (lastSpaceState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // Random Location For Car
		{
			carPosition.x = (rand() % (gridSize)) - (gridSize / 2);
			carPosition.z = (rand() % (gridSize)) - (gridSize / 2);
		}
		lastSpaceState = glfwGetKey(window, GLFW_KEY_SPACE);

		if (lastXState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			if (textured) {
				textured = false;
			}
			else {
				textured = true;
			}
		}
		lastXState = glfwGetKey(window, GLFW_KEY_X);


		//Mouse Movement Calculations
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 60.0f;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = glm::max(-85.0f, glm::min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}


		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
		normalize(cameraSideVector);
		float radius = 5.f;
		vec3 pos = cameraPosition - vec3(radius * cosf(phi)*cosf(theta), radius* sinf(phi), -radius * cosf(phi)*sinf(theta));

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (dy < 0) {
				cameraPosition -= 30.0f * cameraLookAt* dt;
			}
			else if (dy > 0) {
				cameraPosition += 30.0f * cameraLookAt* dt;
			}
		}	
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { //pan X direction
			if (dx < 0) {
				cameraPosition -= 15.0f * cameraSideVector* dt;
			}
			else if (dx > 0) {
				cameraPosition += 15.0f * cameraSideVector* dt;
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) { //pan Y direction
			if (dy < 0) {
				cameraPosition -= 15.0f * cameraUp* dt;
			}
			else if (dy > 0) {
				cameraPosition += 15.0f * cameraUp* dt;
			}
		}
		else {

			cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
			cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
		}

		//First Person Camera
		mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		setViewMatrix(texturedShaderProgram, viewMatrix);
		setViewMatrix(colorShaderProgram, viewMatrix);
		setViewMatrix(lightingShaderProgram, viewMatrix);



	}

	glfwTerminate();

	return 0;
}

