#pragma once

#include "ParsingHelper.h"

#include <vector>

#include <glm/glm.hpp>

class Animation;

class Model
{
public:
	Model();
	virtual ~Model();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	void Load(ci_istringstream& iss);

	virtual glm::mat4 GetWorldMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angleDegrees);
	void SetColour(glm::vec3 colour);

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetScaling() const { return scaling; }
	glm::vec3 GetRotationAxis() const { return rotationAxis; }
	float     GetRotationAngle() const { return rotationAngleInDegrees; }
	ci_string GetName() { return name; }

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

	ci_string name; // The model name is mainly for debugging
	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	glm::vec3 colour;
	
	float     rotationAngleInDegrees;

	// Makes the model follow a list of Animation Keys so it's world transform changes over time
	Animation* animation;

	friend class Animation;
};
