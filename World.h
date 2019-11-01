#pragma once
#include <vector>
class Camera;
class Model;
class Animation;
class AnimationKey;

class World
{
public:
	World();
	~World();

	static World* GetInstance();

	void Update(float deltaTime);
	void Draw();

	void LoadScene(const char* scenePath);

	//Animation* FindAnimation(string animName);
	//AnimationKey* FindAnimationKey(string keyName);

	const Camera* GetCurrentCamera() const;
	
private:
	static World* instance;
	//TODO: Add model
	//TODO: Add Animation
	//TODO: Add Camera
	//TODO: Add current Camera
};