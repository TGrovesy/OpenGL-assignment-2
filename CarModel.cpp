
#include "CarModel.h"
#include "Renderer.h"
#include "ShaderUHelper.h"
#include "EventManager.h"
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

CarModel::CarModel(vec3 size) : CubeModel()
{
	CubeModel::CubeModel();
}

CarModel::~CarModel()
{
	CubeModel::CubeModel();
}

vec3 carLookAt(0.0f, 1.0f, 0.0f);

void CarModel::Update(float deltaTime)
{
	//ANIMATION STUFF HERE IF DESIRED E.G. Rotation
	Model::Update(deltaTime);
	float carTheta = radians(carYRotation); 
	carLookAt = vec3(-cosf(0)*sinf(carTheta), sin(0), -cosf(0) * cosf(carTheta)); //x and z flipped and then z inveresed because our car is drawn facing -x axis

	//Move car
	//front and back
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		carPos += 5.0f * carLookAt* deltaTime;
		wheelAngle -= 180.f * deltaTime;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		carPos -= 5.0f * carLookAt* deltaTime;
		wheelAngle += 180.f * deltaTime;
	}
	//rotate
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		carYRotation += 120.f * deltaTime;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		carYRotation -= 120.0f * deltaTime;
	}

	position = carPos;
}

void CarModel::Draw()
{
	Renderer::SetShader(SHADER_TEXTURE_LIGHTING);
	glUseProgram(Renderer::GetShaderProgramID());
	Model::Draw();
	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture(textureName));
	// Draw the Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture("metal"));
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

	//set Material
	GLuint matAmbLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.ambient");
	GLuint matDiffLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.diffuse");
	GLuint matSpecLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.specular");
	GLuint matShinLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.shininess");
	glUniform3fv(matAmbLoc, 1, new float[3]{ matAmbient.x, matAmbient.y, matAmbient.z });
	glUniform3fv(matDiffLoc, 1, new float[3]{ matDiffuse.x, matDiffuse.y, matDiffuse.z });
	glUniform3fv(matSpecLoc, 1, new float[3]{ matSpecular.x, matSpecular.y, matSpecular.z });
	glUniform1f(matShinLoc, matShininess);


	//main body
	mat4 mainBodyWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	mainBodyWorldMatrix *= translate(mat4(1.0f), carPos);
	mainBodyWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	mainBodyWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * 3.0f, GetScaling().y, GetScaling().z * 7.0f));
	
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &mainBodyWorldMatrix[0][0]);
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
	topCarWorldMatrix *= translate(mat4(1.0f), vec3(carPos.x, carPos.y + (1 * carUniformScaler.y), carPos.z));
	topCarWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	topCarWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * 3.0f, carUniformScaler.y * 1.5f, carUniformScaler.z * 4.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &topCarWorldMatrix[0][0]);
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
	roofCarWorldMatrix *= translate(mat4(1.0f), vec3(carPos.x, carPos.y + ((1 * carUniformScaler.y) + 1 * carUniformScaler.y), carPos.z));
	roofCarWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	roofCarWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * 3.0f, GetScaling().y * 0.5f, GetScaling().z * 2.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &roofCarWorldMatrix[0][0]);
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
	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture("tire"));
	//front left
	float wheelScale = 0.8f;
	//glBindVertexArray(cylinderVBO); TODO: MAKE WHEELS CYLINDERS
	mat4 wheelFLWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelFLWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelFLWorldMatrix *= translate(mat4(1.0f), vec3(1.75* carUniformScaler.x, -0.5f* carUniformScaler.y, 2.5* carUniformScaler.z));
	wheelFLWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * wheelScale, GetScaling().y * wheelScale, GetScaling().z * wheelScale));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelFLWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &wheelFLWorldMatrix[0][0]);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 54);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 54);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 54);
	}


	//front right
	mat4 wheelFRWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelFRWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelFRWorldMatrix *= translate(mat4(1.0f), vec3(-1.75* carUniformScaler.x, -0.5f* carUniformScaler.y, 2.5* carUniformScaler.z));
	wheelFRWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * wheelScale, GetScaling().y * wheelScale, GetScaling().z * wheelScale));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelFRWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &wheelFRWorldMatrix[0][0]);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 54);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 54);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 54);
	}
	//back left
	mat4 wheelBLWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelBLWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelBLWorldMatrix *= translate(mat4(1.0f), vec3(1.75 * carUniformScaler.x, -0.5f* carUniformScaler.y, -2.5* carUniformScaler.z));
	wheelBLWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * wheelScale, GetScaling().y * wheelScale, GetScaling().z * wheelScale));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelBLWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &wheelBLWorldMatrix[0][0]);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 54);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 54);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 54);
	}

	//back right
	mat4 wheelBRWorldMatrix = translate(mat4(1.0f), vec3(0.f));
	wheelBRWorldMatrix *= translate(mat4(1.0f), vec3(carPos[0], carPos[1], carPos[2]));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(carYRotation), vec3(0.0f, 1.0f, 0.0f));
	wheelBRWorldMatrix *= translate(mat4(1.0f), vec3(-1.75* carUniformScaler.x, -0.5f* carUniformScaler.y, -2.5* carUniformScaler.z));
	wheelBRWorldMatrix *= scale(mat4(1.0f), vec3(GetScaling().x * wheelScale, GetScaling().y * wheelScale, GetScaling().z * wheelScale));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	wheelBRWorldMatrix *= rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &wheelBRWorldMatrix[0][0]);
	if (drawMode == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 54);
	}
	else if (drawMode == 1) {
		glDrawArrays(GL_POINTS, 0, 54);
	}
	else if (drawMode == 2) {
		glDrawArrays(GL_LINES, 0, 54);
	}
}


