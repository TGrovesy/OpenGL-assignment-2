#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	void CurrentPlayerPosition(glm::vec3 pos) { playerPosition = pos; };
	void CurrentPlayerRotationY(float rotationY) { playerRotationY = rotationY; };

	virtual void Update(float deltaTime) = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

protected:
	glm::vec3 playerPosition;
	float playerRotationY;
};

