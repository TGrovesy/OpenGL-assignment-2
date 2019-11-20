
#include "ParticleQuad.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

ParticleQuad::ParticleQuad(vec3 size) : Model(), age(0), opacity(1)
{
	textureName = "smokeParticle"; 
	
	// Create Vertex Buffer for alglUniform1f(opacityLocation, opacity);l the verices of the Cube
	vec3 halfSize = size * 0.5f;

	Vertex vertexBuffer[] = {  // position,                normal

		{ vec3(halfSize.x, halfSize.y, 0), vec3(0.0f, 0.0f, 1.0f)  , vec2(1.0f, 1.0f)}, // far 
		{ vec3(-halfSize.x,-halfSize.y,0), vec3(0.0f, 0.0f, 1.0f) , vec2(0.0f, 0.0f)},
		{ vec3(-halfSize.x, halfSize.y,0), vec3(0.0f, 0.0f, 1.0f) , vec2(0.0f, 1.0f)},
									   					    
		{ vec3(halfSize.x, halfSize.y, 0), vec3(0.0f, 0.0f, 1.0f) , vec2(1.0f, 1.0f)},
		{ vec3(halfSize.x,-halfSize.y, 0), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)},
		{ vec3(-halfSize.x,-halfSize.y,0), vec3(0.0f, 0.0f, 1.0f) , vec2(0.0f, 0.0f)}
	};

	// Create a vertex array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferID)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);



	// 1st attribute buffer : vertex Positions
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)0
	);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : vertex normal
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches normal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(vec3) * 2) //offset By two vec 3 values
	);
	glEnableVertexAttribArray(2);

}

ParticleQuad::~ParticleQuad()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleQuad::Update(float deltaTime)
{
	//ANIMATION STUFF HERE IF DESIRED E.G. Rotation
	Model::Update(deltaTime);
	age += deltaTime;
	float interpolationParameter = age / LIFETIME;

	float scalingParam = linear(interpolationParameter);
	float opacityParam = linear(interpolationParameter);

	opacity = OPACITY_I * (1 - opacityParam) + OPACITY_F * opacityParam;
	SetScaling(SCALE_I * (1 - scalingParam) + SCALE_F * scalingParam);
}

void ParticleQuad::Draw()
{
	Renderer::SetShader(SHADER_TEXTURE_LIGHTING);
	glUseProgram(Renderer::GetShaderProgramID());
	GLuint opacityLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "opacity");
	glUniform1f(opacityLocation, opacity);
	Model::Draw();
	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture("smokeParticle"));
	// Draw the Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);



	//set Material
	GLuint matAmbLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.ambient");
	GLuint matDiffLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.diffuse");
	GLuint matSpecLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.specular");
	GLuint matShinLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.shininess");
	glUniform3fv(matAmbLoc, 1, new float[3]{ matAmbient.x, matAmbient.y, matAmbient.z });
	glUniform3fv(matDiffLoc, 1, new float[3]{ matDiffuse.x, matDiffuse.y, matDiffuse.z });
	glUniform3fv(matSpecLoc, 1, new float[3]{ matSpecular.x, matSpecular.y, matSpecular.z });
	glUniform1f(matShinLoc, matShininess);
	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices: 3 * 2 * 1 
}

bool ParticleQuad::ParseLine(const std::vector<std::string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}

bool ParticleQuad::expired() const {
	if (age <= LIFETIME) {
		return false;
	}
	else {
		return true;
	}
}


float ParticleQuad::linear(float t) { return t; }
float ParticleQuad::quadratic(float t) { return t * t; }
float ParticleQuad::root(float t) { return sqrt(t); }
float ParticleQuad::smooth(float t) {
	float theta = glm::pi<float>() * (1 + t);
	return (glm::cos(theta) + 1) / 2;
}

