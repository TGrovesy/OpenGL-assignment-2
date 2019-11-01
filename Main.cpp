#include "EventManager.h"
#include "Renderer.h"

int main(int argc, char*argv[]) {
	EventManager::Initialize();
	Renderer::Initialize();

	//TODO: Create Scene Loader

	if (argc > 1) {
		//Load Scene from command line
	} else {
		//
	#if defined(PLATFORM_OSX)		
		//		world.LoadScene("Scenes/StaticScene.scene");
		//		world.LoadScene("Scenes/CoordinateSystem.scene");
	#else
		//		world.LoadScene("../Assets/Scenes/StaticScene.scene");
		//		world.LoadScene("../Assets/Scenes/CoordinateSystem.scene");
	#endif
	}

	//Main Loop
	do {
		//Update eventmanager
		EventManager::Update();

		//TODO: Update World
		float deltaTime = EventManager::GetFrameTime();
		
		//TODO: Draw World
	} while (!EventManager::ExitRequested()); //TODO: Change to event manager request exit

	Renderer::Shutdown();
	EventManager::Shutdown();
	return 0;
}