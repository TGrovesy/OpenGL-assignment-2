#pragma once
struct GLFWwindow;

class EventManager
{
public:
	static void Initialize();
	static void Shutdown();

	static void Update();

	static float GetFrameTime();
	static bool ExitRequested();

	static GLFWwindow* GetWindow();

	static float GetMouseMotionX();
	static float GetMouseMotionY();

	static void EnableMouseCursor();
	static void DisableMouseCursor();
	static float GetRandomFloat(float min, float max);

private:
	// Time
	static double sLastFrameTime;
	static float sFrameTime;

	// Mouse
	static double sLastMousePositionX;
	static float  sMouseDeltaX;
	static double sLastMousePositionY;
	static float  sMouseDeltaY;

	// Window
	static GLFWwindow* spWindow;
};
