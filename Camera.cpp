#include "Camera.h"
#include <glm/gtx/transform.hpp>

using namespace glm;

Camera::Camera()
{
}


Camera::~Camera()
{
}

mat4 Camera::GetViewProjectionMatrix() const {
	mat4 viewProjection(1.0f);
	viewProjection = GetProjectionMatrix() * GetViewMatrix();
	return viewProjection;
}

mat4 Camera::GetProjectionMatrix() const
{
	return perspective(radians(70.0f), 4.0f / 3.0f, 0.1f, 100.0f); //fov, aspect ratio, near plane, far plane (view Distance)
}
