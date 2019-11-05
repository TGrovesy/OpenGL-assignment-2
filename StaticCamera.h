#pragma once

#include "Camera.h"

class StaticCamera : public Camera
{
public:
	StaticCamera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 upVector);
	virtual ~StaticCamera();

	virtual void Update(float deltaTime);

	virtual glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 upVector;
};
