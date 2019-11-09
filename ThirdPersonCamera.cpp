#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "Renderer.h"
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

	// Mouse motion to get the variation in angle
	horizontalAngle -= EventManager::GetMouseMotionX() * angularSpeed * dt;
	verticalAngle -= EventManager::GetMouseMotionY() * angularSpeed * dt;

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

	//set viewPos

	GLuint camViewPosLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewPos");
	glUniform3fv(camViewPosLoc, 1, new float[3]{ position.x, position.y, position.z });

	//Camera Controll
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		position += lookAt * dt * speed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= lookAt * dt * speed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		position += sideVector * dt * speed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= sideVector * dt * speed;
	}
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(position, position + lookAt, vec3(0.0f, 1.0f, 0.0f));
}
