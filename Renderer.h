#pragma once

struct GLFWwindow;

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>
#include <map>
#include"ParsingHelper.h"


enum ShaderType{
	SHADER_SOLID_COLOUR,
	SHADER_TEXTURE_LIGHTING
};

class Renderer {
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static GLuint LoadShaders(std::string vertexShaderPath, std::string fragShaderPath);
	
	static unsigned int GetShaderProgramID(){ return shaderProgramID[currentShader]; }
	static unsigned int GetCurrentShader() { return currentShader; }
	static void SetShader(ShaderType type);
	static void CheckForErrors();
	static bool PrintError();
	static int GetTexture(std::string texture);
protected:

	static std::map<std::string, int>* textures;
private:
	static GLFWwindow* window;

	static std::vector<unsigned int> shaderProgramID;
	static unsigned int currentShader;

	static void LoadAllTextures();
	static int LoadTexture(char* imagepath);
};
