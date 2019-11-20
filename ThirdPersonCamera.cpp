#include "ThirdPersonCamera.h"
#include "CarModel.h"
#include "EventManager.h"
#include "Renderer.h"
#include "World.h"
#include "Main.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 pos) : Camera(), position(pos), lookAt(0.0f, 0.0f, -1.0f), horizontalAngle(90.0f), verticalAngle(0.0f), speed(5.0f), angularSpeed(2.5f)
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();



	// Clamp vertical angle to [-85, 85] degrees
	verticalAngle = std::max(-85.0f, std::min(85.0f, verticalAngle));
	if (horizontalAngle > 360)
	{
		horizontalAngle -= 360;
	}
	else if (horizontalAngle < -360)
	{
		horizontalAngle += 360;
	}

	float theta = radians(horizontalAngle);
	float phi = radians(verticalAngle);
	lookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));

	vec3 sideVector = glm::cross(lookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);

	vec3 upVector(0, 1, 0);

	//set viewPos

	GLuint camViewPosLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewPos");
	glUniform3fv(camViewPosLoc, 1, new float[3]{ position.x, position.y, position.z });
	
	//Camera Controll
	/*
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (EventManager::GetMouseMotionY() < 0) {
			position += 30.0f * lookAt* dt;
		}
		else if (EventManager::GetMouseMotionY() > 0) {
			position -= 30.0f * lookAt* dt;
		}
	}
	else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (EventManager::GetMouseMotionX() < 0) {
			position -= 15.0f * sideVector* dt;
		}
		else if (EventManager::GetMouseMotionX() > 0) {
			position += 15.0f * sideVector* dt;
		}
	}
	else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) { //pan Y direction
		if (EventManager::GetMouseMotionY() < 0) {
			position += 15.0f * upVector* dt;
		}
		else if (EventManager::GetMouseMotionY() > 0) {
			position -= 15.0f * upVector* dt;
		}
	}
	else {

		// Mouse motion to get the variation in angle
		horizontalAngle -= EventManager::GetMouseMotionX() * angularSpeed * dt;
		verticalAngle -= EventManager::GetMouseMotionY() * angularSpeed * dt;
	}*/

	// Mouse motion to get the variation in angle

	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		horizontalAngle -= EventManager::GetMouseMotionX() * angularSpeed * dt;
	}
	else {
		horizontalAngle = playerRotationY + 90;
	}
	verticalAngle -= EventManager::GetMouseMotionY() * angularSpeed * dt;
	position = playerPosition;
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	float theta = radians(horizontalAngle);
	float phi = radians(verticalAngle);
	float radius = 9.0f;
	vec3 lookAtPos = position - vec3(radius * cosf(phi)*cosf(theta),radius * sinf(phi), radius * -cosf(phi)*sinf(theta));
	return glm::lookAt(lookAtPos, position + lookAt, vec3(0.0f, 1.0f, 0.0f));
}
