#include "Main.h"
#include "EventManager.h"
#include "Renderer.h"
#include "World.h"

Main::Main() {

}

Main::~Main() {

}


int main(int argc, char*argv[]) {
	EventManager::Initialize();
	Renderer::Initialize();
	World world;

	if (argc > 1) {
		//Load Scene from command line
	} else {
		//
	#if defined(PLATFORM_OSX)		
		world.LoadScene("Scenes/scene1.scene");
		//		world.LoadScene("Scenes/CoordinateSystem.scene");
	#else
		world.LoadScene("../Source/OpenGL-assignment-2/Assets/Scenes/scene1.scene");
		//		world.LoadScene("../Assets/Scenes/CoordinateSystem.scene");
	#endif
	}

	//Main Loop
	do {
		//Update eventmanager
		EventManager::Update();

		//Update World
		float deltaTime = EventManager::GetFrameTime();
		world.Update(deltaTime);
		
		//Draw World
		world.Draw();
	} while (!EventManager::ExitRequested()); 

	Renderer::Shutdown();
	EventManager::Shutdown();
	return 0;
}