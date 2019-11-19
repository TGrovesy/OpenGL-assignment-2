#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 pos);
	virtual ~FirstPersonCamera();

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

#pragma once
