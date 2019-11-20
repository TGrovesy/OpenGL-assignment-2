#pragma once
#include <vector>
class Camera;
class Model;
class ParticleQuad;
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
	std::vector<ParticleQuad*> particles;
	std::vector<Animation*> animation;
	std::vector<AnimationKey*> animationKey;
	std::vector<Camera*> camera;
	unsigned int currentCamera;
	Model* playerCar;
	float spawnTimer; 
	const float SPAWN_PERIOD = 0.25;
};