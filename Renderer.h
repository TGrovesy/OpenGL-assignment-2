#pragma once

struct GLFWwindow;

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>


enum ShaderType{
	SHADER_SOLID_COLOUR
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

private:
	static GLFWwindow* window;

	static std::vector<unsigned int> shaderProgramID;
	static unsigned int currentShader;
};
