
#include "LightModel.h"
#include "Renderer.h"

using namespace glm;

LightModel::LightModel(vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Cube
	vec3 halfSize = size * 0.5f;

	Vertex vertexBuffer[] = {  // position,                normal
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)}, //left
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)},
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)},

								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)},
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)},

								{ vec3(halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f)  , vec2(1.0f, 1.0f)}, // far 
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f) , vec2(0.0f, 0.0f)},
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f) , vec2(0.0f, 1.0f)},

								{ vec3(halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f) , vec2(1.0f, 1.0f)},
								{ vec3(halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f), vec2(1.0f, 0.0f)},
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f, 0.0f,-1.0f) , vec2(0.0f, 0.0f)},

								{ vec3(halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f,-1.0f, 0.0f) , vec2(1.0f, 1.0f), }, // bottom 
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f,-1.0f, 0.0f), vec2(0.0f, 0.0f), },
								{ vec3(halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f,-1.0f, 0.0f) ,  vec2(1.0f, 0.0f) },

								{ vec3(halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f,-1.0f, 0.0f),  vec2(1.0f, 1.0f)  },
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f,-1.0f, 0.0f), vec2(0.0f, 1.0f), },
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(0.0f,-1.0f, 0.0f), vec2(0.0f, 0.0f), },

								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f),vec2(0.0f, 1.0f) }, // near 
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f),vec2(0.0f, 0.0f) },
								{ vec3(halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f) ,vec2(1.0f, 0.0f)},

								{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f) ,vec2(1.0f, 1.0f) },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f),vec2(0.0f, 1.0f) },
								{ vec3(halfSize.x,-halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) },

								{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(1.0f, 1.0f)}, // right 
								{ vec3(halfSize.x,-halfSize.y,-halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(0.0f, 0.0f)},
								{ vec3(halfSize.x, halfSize.y,-halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(1.0f, 0.0f)},

								{ vec3(halfSize.x,-halfSize.y,-halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(0.0f, 0.0f)},
								{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(1.0f, 1.0f)},
								{ vec3(halfSize.x,-halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f),vec2(0.0f, 1.0f)},

								{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f)  ,vec2(10.0f, 10.0f) }, // top 
								{ vec3(halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 1.0f, 0.0f)  ,vec2(10.0f, 0.0f) },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 1.0f, 0.0f) ,vec2(0.0f, 0.0f)  },

								{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f)  ,vec2(10.0f, 10.0f) },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(0.0f, 1.0f, 0.0f) ,vec2(0.0f, 0.0f) },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f) ,vec2(0.0f, 10.0f) }
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

LightModel::~LightModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void LightModel::Update(float deltaTime)
{
	//ANIMATION STUFF HERE IF DESIRED E.G. Rotation
	Model::Update(deltaTime);
}

void LightModel::Draw()
{
	Renderer::SetShader(SHADER_TEXTURE_LIGHTING);
	glUseProgram(Renderer::GetShaderProgramID());
	Model::Draw();
	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture(textureName));
	// Draw the Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);


	//set colour
	GLuint colourLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "inColour");
	float colours[] = { colour.r, colour.g, colour.b, 1.0 };
	glUniform4fv(colourLoc, 1, colours);

	//set light
	GLuint lightAmbLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "light.ambient");
	GLuint lightDiffLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "light.diffuse");
	GLuint lightSpecLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "light.specular");
	GLuint lightPosLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "light.position");
	GLuint lightPLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightPos");
	glUniform3fv(lightAmbLoc, 1, new float[3]{ lightAmbient.x, lightAmbient.y, lightAmbient.z});
	glUniform3fv(lightDiffLoc, 1, new float[3]{ lightDiffuse.x, lightDiffuse.y, lightDiffuse.z });
	glUniform3fv(lightSpecLoc, 1, new float[3]{ lightSpecular.x, lightSpecular.y, lightSpecular.z });
	glUniform3fv(lightPosLoc, 1, new float[3]{ lightPosition.x, lightPosition.y, lightPosition.z});
	glUniform3fv(lightPLoc, 1, new float[3]{ lightPosition.x, lightPosition.y, lightPosition.z });


	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 
}

bool LightModel::ParseLine(const std::vector<std::string> &token)
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
