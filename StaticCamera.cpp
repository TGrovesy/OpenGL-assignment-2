#include "StaticCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "EventManager.h"

using namespace glm;

StaticCamera::StaticCamera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 upVector)
	: Camera(), position(pos), lookAt(lookAt), upVector(upVector)
{
}

StaticCamera::~StaticCamera()
{
}

void StaticCamera::Update(float dt)
{
	EventManager::EnableMouseCursor();
}

glm::mat4 StaticCamera::GetViewMatrix() const
{
	return glm::lookAt(position,		// Camera position
		lookAt,	// Look towards this point
		upVector		// Up vector
	);
}
