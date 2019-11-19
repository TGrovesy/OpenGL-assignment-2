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

	void Load(std::istringstream& iss);

	virtual glm::mat4 GetWorldMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angleDegrees);
	void SetColour(glm::vec3 colour);

	//Material
	void SetMaterialAmbient(glm::vec3 ambient);
	void SetMaterialDiffuse(glm::vec3 diffuse);
	void SetMaterialSpecular(glm::vec3 specular);

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetScaling() const { return scaling; }
	glm::vec3 GetRotationAxis() const { return rotationAxis; }
	float     GetRotationAngle() const { return rotationAngleInDegrees; }
	std::string GetName() { return name; }

	

protected:
	virtual bool ParseLine(const std::vector<std::string> &token) = 0;


	std::string name; // The model name is mainly for debugging
	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	glm::vec3 colour;
	std::string textureName;
	glm::vec3 matAmbient = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 matDiffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 matSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	float matShininess = 32.0f;
	glm::vec3 lightAmbient = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 lightDiffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 lightSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
	
	float     rotationAngleInDegrees;

	// Makes the model follow a list of Animation Keys so it's world transform changes over time
	Animation* animation;

	friend class Animation;
};
