#pragma once

#include "Camera.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(glm::vec3 pos);
	virtual ~ThirdPersonCamera();

	virtual void Update(float deltaTime);

	virtual glm::mat4 GetViewMatrix() const;
private:
	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;

	float speed;
	float angularSpeed;

	glm::vec3 lookAt;
};

