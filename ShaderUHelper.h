#pragma once
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/common.hpp>
class ShaderUHelper {
public:
	static void SetVec3(int shaderProgram, char* location, glm::vec3 value);
	static void SetMat4(int shaderProgram, char* location, glm::mat4 value);
};