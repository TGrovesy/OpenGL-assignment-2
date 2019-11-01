#include "World.h"
#include "EventManager.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
using namespace std;

World* World::instance;

World::World() {
	instance = this;

	//TODO: setup camera
}

World::~World() {
	//TODO: Clear models, animations and camera
}

World* World::GetInstance() {
	return instance;
}

void World::Update(float deltaTime) {
	//TODO: Use event manager to handle various inputs
}

void World::Draw() {
	Renderer::BeginFrame();

	//set shader
	glUseProgram(Renderer::GetShaderProgramID());

	GLuint viewProjectionMatrixLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewMatrix");
	
	//TODO: set view Prjection from camera

	//TODO: Draw Models

	//Once done with shader ensure we set it as previous
	unsigned int prevShader = Renderer::GetCurrentShader();

	//Check for errors
	Renderer::CheckForErrors();

	//Restore Previous Shader
	Renderer::SetShader((ShaderType)prevShader);
	Renderer::EndFrame();
}

//Load Scene
void World::LoadScene(const char * scenePath) {
	//TODO: All scene stuff
}

/*
Animation* World::FindAnimation(string animName) {
	//TODO: find animation
}

AnimationKey* World::FindAnimationKey(string keyName) {

}*/

const Camera* World::GetCurrentCamera() const {
	//TODO: return current camera
	return nullptr;
}