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

	std::vector<Model*> model;
	std::vector<Animation*> animation;
	std::vector<AnimationKey*> animationKey;
	std::vector<Camera*> camera;
	unsigned int currentCamera;
};