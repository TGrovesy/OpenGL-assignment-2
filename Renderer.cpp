#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "EventManager.h"
#include "ParsingHelper.h"

#include <GLFW/glfw3.h>

#include <FreeImageIO.h>
#include <map>

#if defined(PLATFORM_OSX)
#define fscanf_s fscanf
#endif

using namespace std;

vector<unsigned int> Renderer::shaderProgramID;
unsigned int Renderer::currentShader;

GLFWwindow* Renderer::window = nullptr;

std::map<std::string, int>* Renderer::textures = new std::map<std::string, int>();

GLuint ERROR_TEXTURE;

void Renderer::Initialize() {
	window = EventManager::GetWindow();
	glfwMakeContextCurrent(window);

	//Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW Failed to initialize!\n");
		getchar();
		exit(-1);
	}
	LoadAllTextures();

	glGetError(); // deals with random GLEW error on initializeation

	glClearColor(0.f, 0.f, 0.f, 0.f); //Clear black

	//Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Enable Face culling
	glEnable(GL_CULL_FACE);

	CheckForErrors();

	//Load Shaders
#if defined(PLATFORM_OSX)
	std::string shaderPathPrefix = "Shaders/";
#else
	std::string shaderPathPrefix = "../Source/OpenGL-assignment-2/Assets/Shaders/";
#endif
	shaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "SolidColor.vertexshader",
			shaderPathPrefix + "SolidColor.fragmentshader")
	); 
	shaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "LightingShader.vertexshader",
			shaderPathPrefix + "LightingShader.fragmentshader")
	);


	glActiveTexture(GL_TEXTURE0);
	GLuint textureSampler = glGetUniformLocation(shaderProgramID[1], "textureSampler"); //index 1 is ourlighting shader
	glUniform1i(textureSampler, 0);
} 

void Renderer::Shutdown()
{
	// Shaders
	for (vector<unsigned int>::iterator it = shaderProgramID.begin(); it < shaderProgramID.end(); ++it)
	{
		glDeleteProgram(*it);
	}
	shaderProgramID.clear();


	// Managed by EventManager so dont do anything to window other than unset object variable
	window = nullptr;
}

void Renderer::BeginFrame()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndFrame()
{
	// Swap buffers
	glfwSwapBuffers(window);

	CheckForErrors();
}

void Renderer::SetShader(ShaderType type)
{
	if (type < (int)shaderProgramID.size())
	{
		currentShader = type;
	}
}

//
// The following code is taken from
// www.opengl-tutorial.org
//
GLuint Renderer::LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

bool Renderer::PrintError()
{
	static bool checkForErrors = true;

	if (!checkForErrors)
	{
		return false;
	}

	const char * errorString = NULL;
	bool retVal = false;

	switch (glGetError())
	{
	case GL_NO_ERROR:
		retVal = true;
		break;

	case GL_INVALID_ENUM:
		errorString = "GL_INVALID_ENUM";
		break;

	case GL_INVALID_VALUE:
		errorString = "GL_INVALID_VALUE";
		break;

	case GL_INVALID_OPERATION:
		errorString = "GL_INVALID_OPERATION";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;

	case GL_OUT_OF_MEMORY:
		errorString = "GL_OUT_OF_MEMORY";
		break;

	default:
		errorString = "UNKNOWN";
		break;
	}

	if (!retVal)
	{
		printf("%s\n", errorString);
	}

	return retVal;
}


void Renderer::CheckForErrors()
{
	while (PrintError() == false)
	{
	}
}

int Renderer::LoadTexture(char* imagepath)
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

void Renderer::LoadAllTextures() {
	
	std::string texturePathPrefix = "../Source/OpenGL-assignment-2/Assets/Textures/";
#if defined(PLATFORM_OSX)
	GLuint brickTextureID = LoadTexture("Textures/brick.jpg");
	GLuint cementTextureID = LoadTexture("Textures/cement.jpg");
	GLuint grassTextureID = LoadTexture("Textures/grass.jpg");
	GLuint metalTextureID = LoadTexture("Textures/metal.jpg");
	GLuint tireTextureID = LoadTexture("Textures/tire.jpg");
#else
	GLuint brickTextureID = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/brick.jpg");
	GLuint cementTextureID = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/cement.jpg");
	GLuint grassTextureID = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/grass.jpg");
	GLuint metalTextureID = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/metal.jpg");
	GLuint tireTextureID = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/tire.jpg");
	ERROR_TEXTURE = LoadTexture("../Source/OpenGL-assignment-2/Assets/Textures/ERROR.jpg");
#endif

	textures->insert(pair<char*, int>("grass", grassTextureID));
	textures->insert(pair<char*, int>("cement", cementTextureID));
	textures->insert(pair<char*, int>("brick", brickTextureID));
	textures->insert(pair<char*, int>("metal", metalTextureID));
	textures->insert(pair<char*, int>("tire", tireTextureID));
}

int Renderer::GetTexture(std::string texture) {
	try {
		return textures->at(texture);
	}
	catch (exception e) {
		return 0;
	}
}