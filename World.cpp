#include "World.h"
#include "EventManager.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "ThirdPersonCamera.h"

#include "Model.h"
#include "CubeModel.h"
#include "LightModel.h"

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

World* World::instance;

World::World() {
	instance = this;

	//TODO: setup camera
	camera.push_back(new ThirdPersonCamera(vec3(3.0f, 5.0f, 10.0f))); //camera 0
	camera.push_back(new StaticCamera(vec3(0, 20, 0), vec3(0, 0, 0), vec3(0, 0, -1)));
	currentCamera = 0;
}

World::~World() {
	// Models
	for (vector<Model*>::iterator it = model.begin(); it < model.end(); ++it)
	{
		delete *it;
	}
	model.clear();

	//TODO: clear animation and animation keys

	// Camera
	for (vector<Camera*>::iterator it = camera.begin(); it < camera.end(); ++it)
	{
		delete *it;
	}
	camera.clear();
}

World* World::GetInstance() {
	return instance;
}

void World::Update(float deltaTime) {
	//TODO: Use event manager to handle various inputs

	//Update current Camera
	camera[currentCamera]->Update(deltaTime);

	//Update Models
	for (vector<Model*>::iterator it = model.begin(); it < model.end(); ++it) {
		(*it)->Update(deltaTime);
	}
}

void World::Draw() {
	Renderer::BeginFrame();

	//set shader
	glUseProgram(Renderer::GetShaderProgramID());

	GLuint viewProjectionMatrixLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewProjectionMatrix");
	
	//set view Prjection from camera
	mat4 viewProejection = camera[currentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(viewProjectionMatrixLoc, 1, GL_FALSE, &viewProejection[0][0]);

	//Draw Models
	for (vector<Model*>::iterator it = model.begin(); it < model.end(); ++it) {
		(*it)->Draw();
	}

	//Once done with shader ensure we set it as previous
	unsigned int prevShader = Renderer::GetCurrentShader();

	//Check for errors
	Renderer::CheckForErrors();

	//Restore Previous Shader
	Renderer::SetShader((ShaderType)prevShader);
	Renderer::EndFrame();
}

//Load Scene

void World::LoadScene(const char * scene_path) {
	// Using case-insensitive strings and streams for easier parsing
	ifstream input;
	input.open(scene_path, ios::in);
	
	// Invalid file
	if (input.fail())
	{
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}
	string item;
	while (std::getline(input, item, '['))
	{
		//printf(item.c_str());
		istringstream iss(item);

		string result;
		if (std::getline(iss, result, ']'))
		{
			if (result == "Cube")
			{
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				model.push_back(cube);

			}
			else if (result == "Light") {
				LightModel* light = new LightModel();
				light->Load(iss);
				model.push_back(light);

			}
			else if (result.empty() == false && result[0] == '#')
			{
				// this is defines a comment line in our scene file
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
		}
	}
	input.close();
}

/*
Animation* World::FindAnimation(string animName) {
	//TODO: find animation
}

AnimationKey* World::FindAnimationKey(string keyName) {

}*/

const Camera* World::GetCurrentCamera() const {
	//return current camera
	return camera[currentCamera];
}