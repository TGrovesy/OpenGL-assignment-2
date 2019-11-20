#pragma once

#include "CubeModel.h"
#include "ParticleQuad.h"
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
using namespace glm;

class CarModel : public CubeModel
{
public:
	CarModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f)); //default size 1
	virtual ~CarModel();

	virtual void Update(float deltaTime);
	virtual void Draw();

	vec3 carPos = vec3(0.0f, 3.0f, 0.0f);
	float carYRotation = 0.0f;

	int drawMode = 0;
	vec3 carUniformScaler = vec3(1.0f, 1.0f, 1.0f);
	float wheelScale = 0.5f;
	float wheelAngle = 0.0f;

protected:

private:

	float spawnTimer;
	const float SPAWN_PERIOD = 0.25;
	std::vector<ParticleQuad*> exhaustParticles;

};