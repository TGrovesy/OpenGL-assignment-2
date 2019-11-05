#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update(float deltaTime) = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

	
};

