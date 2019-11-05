#include "Model.h"
#include "World.h"
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>

using namespace std;
using namespace glm;



Model::Model()
	: name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scaling(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 1.0f, 0.0f),
	rotationAngleInDegrees(0.0f), animation(nullptr)
{
}

Model::~Model()
{
}

void Model::Update(float deltaTime)
{
}

void Model::Draw()
{
}

void Model::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while (std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}

/*Defines how we read in our scene file forour models*/
bool Model::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty() == false)
	{
		if (token[0].empty() == false && token[0][0] == '#')
		{
			return true;
		}
		else if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			name = token[2];
		}
		else if (token[0] == "position")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			position.x = static_cast<float>(atof(token[2].c_str()));
			position.y = static_cast<float>(atof(token[3].c_str()));
			position.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "rotation")
		{
			assert(token.size() > 5);
			assert(token[1] == "=");

			rotationAxis.x = static_cast<float>(atof(token[2].c_str()));
			rotationAxis.y = static_cast<float>(atof(token[3].c_str()));
			rotationAxis.z = static_cast<float>(atof(token[4].c_str()));
			rotationAngleInDegrees = static_cast<float>(atof(token[5].c_str()));

			glm::normalize(rotationAxis);
		}
		else if (token[0] == "scaling")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			scaling.x = static_cast<float>(atof(token[2].c_str()));
			scaling.y = static_cast<float>(atof(token[3].c_str()));
			scaling.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "animation")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string animName = token[2];

			//animation = World::GetInstance()->FindAnimation(animName);
		}
		else if (token[0] == "colour") {

			assert(token.size() > 4);
			assert(token[1] == "=");
			colour.r = static_cast<float>(atof(token[2].c_str()));
			colour.g = static_cast<float>(atof(token[3].c_str()));
			colour.b = static_cast<float>(atof(token[4].c_str()));

		}
		else
		{
			return false;
		}
	}

	return true;
}

glm::mat4 Model::GetWorldMatrix() const
{
	mat4 worldMatrix(1.0f);

	if (animation)
	{
		// Get world transform from animation key frames / current time
		//worldMatrix = mAnimation->GetAnimationWorldMatrix();
	}
	else
	{
		mat4 t = glm::translate(mat4(1.0f), position);
		mat4 r = glm::rotate(mat4(1.0f), glm::radians(rotationAngleInDegrees), rotationAxis);
		mat4 s = glm::scale(mat4(1.0f), scaling);
		worldMatrix = t * r * s;
	}


	return worldMatrix;
}

void Model::SetPosition(glm::vec3 position)
{
	position = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	scaling = scaling;
}

void Model::SetColour(glm::vec3 colour)
{
	colour = colour;
}


void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	rotationAxis = axis;
	rotationAngleInDegrees = angleDegrees;
}
