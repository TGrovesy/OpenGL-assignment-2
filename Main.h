#pragma once
#include "World.h"
class Main{
public:
	Main();
	virtual ~Main();
	World GetWorld() { return world; };

	World world;
	static Main instance;
private:
};