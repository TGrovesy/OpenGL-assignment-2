
#include "CubeModel.h"
#include "Renderer.h"

using namespace glm;

CubeModel::CubeModel(vec3 size) : Model()
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

CubeModel::~CubeModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void CubeModel::Update(float deltaTime)
{ 
	//ANIMATION STUFF HERE IF DESIRED E.G. Rotation
	Model::Update(deltaTime);
}

void CubeModel::Draw()
{
	Renderer::SetShader(SHADER_TEXTURE_LIGHTING);
	glUseProgram(Renderer::GetShaderProgramID());
	glBindTexture(GL_TEXTURE_2D, Renderer::GetTexture(textureName));
	// Draw the Vertex Buffer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	//set color
	GLuint colourLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "inColour");
	float colours[] = { colour.r, colour.g, colour.b, 1.0 };
	glUniform4fv(colourLoc, 1, colours);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 
}

bool CubeModel::ParseLine(const std::vector<std::string> &token)
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
