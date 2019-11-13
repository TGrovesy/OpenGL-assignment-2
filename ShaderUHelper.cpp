#include "ShaderUHelper.h";
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/common.hpp>
#include <GL/glew.h>   

void ShaderUHelper::SetVec3(int shaderProgram, char* loc, glm::vec3 value) {
	//glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniform3fv(pos, 1, new float[3]{ value.x, value.y, value.z });
}

void ShaderUHelper::SetMat4(int shaderProgram, char* loc, glm::mat4 value) {
	//glUseProgram(shaderProgram);
	GLuint pos = glGetUniformLocation(shaderProgram, loc);
	glUniformMatrix4fv(pos, 1, GL_FALSE, &value[0][0]);
}