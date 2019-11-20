#pragma once

#include "Model.h"

class ParticleQuad : public Model
{
public:
	ParticleQuad(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f)); //default size 1
	virtual ~ParticleQuad();

	bool expired() const;
	virtual void Update(float deltaTime) override;
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<std::string> &token);
	unsigned int VAO; //Vertex array 
	unsigned int VBO; //vertex buffer
private:
	float age, opacity;

	const glm::vec3 SCALE_I = glm::vec3(0, 0, 0);
	const glm::vec3 SCALE_F = glm::vec3(2, 2, 2);
	const float OPACITY_I = 1;
	const float OPACITY_F = 0;
	const float LIFETIME = 5; // in seconds


	static float linear(float t);
	static float quadratic(float t);
	static float root(float t);
	static float smooth(float t);

	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 UV;
	};

};