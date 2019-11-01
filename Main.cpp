#include "EventManager.h"
#include "Renderer.h"
#include "World.h"

int main(int argc, char*argv[]) {
	EventManager::Initialize();
	Renderer::Initialize();

	//World
	World world;

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