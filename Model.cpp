#include "Model.h"
#include "World.h"
#include "Renderer.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
using namespace std;
using namespace glm;



Model::Model()
	: name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scaling(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 1.0f, 0.0f),
	rotationAngleInDegrees(0.0f), animation(nullptr), velocity(0.f)
{
}

Model::~Model()
{
}

void Model::Update(float deltaTime)
{
	position += velocity * deltaTime;
}

void Model::Draw()
{
	//set colour
	GLuint colourLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "inColour");
	float colours[] = { colour.r, colour.g, colour.b, 1.0 };
	glUniform4fv(colourLoc, 1, colours);

}

void Model::Load(istringstream& iss)
{
	string line;

	// Parse model line by line
	while (std::getline(iss, line))
	{
		// Splitting line into tokens
		istringstream strstr(line);
		istream_iterator<string, char> it(strstr);
		istream_iterator<string, char> end;
		vector<string> token(it, end);
		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}

/*Defines how we read in our scene file forour models*/
bool Model::ParseLine(const vector<string> &token)
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

			string animName = token[2];

			//animation = World::GetInstance()->FindAnimation(animName);
		}
		else if (token[0] == "colour") {

			assert(token.size() > 4);
			assert(token[1] == "=");
			colour.r = static_cast<float>(atof(token[2].c_str()));
			colour.g = static_cast<float>(atof(token[3].c_str()));
			colour.b = static_cast<float>(atof(token[4].c_str()));

		}
		else if (token[0] == "texture") {

			assert(token.size() > 2);
			assert(token[1] == "=");

			textureName = token[2];
		}
		else if (token[0] == "material.ambient") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			matAmbient.x = static_cast<float>(atof(token[2].c_str()));
			matAmbient.y = static_cast<float>(atof(token[3].c_str()));
			matAmbient.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "material.diffuse") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			matDiffuse.x = static_cast<float>(atof(token[2].c_str()));
			matDiffuse.y = static_cast<float>(atof(token[3].c_str()));
			matDiffuse.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "material.specular") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			matSpecular.x = static_cast<float>(atof(token[2].c_str()));
			matSpecular.y = static_cast<float>(atof(token[3].c_str()));
			matSpecular.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "material.shininess") {
			assert(token.size() > 2);
			assert(token[1] == "=");
			matShininess = static_cast<float>(atof(token[2].c_str()));
		}
		else if (token[0] == "light.ambient") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			lightAmbient.x = static_cast<float>(atof(token[2].c_str()));
			lightAmbient.y = static_cast<float>(atof(token[3].c_str()));
			lightAmbient.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "light.diffuse") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			lightDiffuse.x = static_cast<float>(atof(token[2].c_str()));
			lightDiffuse.y = static_cast<float>(atof(token[3].c_str()));
			lightDiffuse.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "light.specular") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			lightSpecular.x = static_cast<float>(atof(token[2].c_str()));
			lightSpecular.y = static_cast<float>(atof(token[3].c_str()));
			lightSpecular.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "light.position") {
			assert(token.size() > 4);
			assert(token[1] == "=");
			lightPosition.x = static_cast<float>(atof(token[2].c_str()));
			lightPosition.y = static_cast<float>(atof(token[3].c_str()));
			lightPosition.z = static_cast<float>(atof(token[4].c_str()));
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
	this->position = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	scaling = scaling;
}

void Model::SetColour(glm::vec3 colour)
{
	colour = colour;
}

void Model::SetMaterialAmbient(glm::vec3 ambient)
{
	matAmbient = ambient;
}

void Model::SetMaterialSpecular(glm::vec3 specular)
{
	matSpecular = specular;
}

void Model::SetMaterialDiffuse(glm::vec3 diffuse)
{
	matDiffuse = diffuse;
}



void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	rotationAxis = axis;
	rotationAngleInDegrees = angleDegrees;
}

void Model::SetVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
}
