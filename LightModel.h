#pragma once

#include "Model.h"

class LightModel : public Model
{
public:
	LightModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f)); //default size 1
	virtual ~LightModel();

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<std::string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 UV;
	};

	unsigned int VAO; //Vertex array 
	unsigned int VBO; //vertex buffer
};