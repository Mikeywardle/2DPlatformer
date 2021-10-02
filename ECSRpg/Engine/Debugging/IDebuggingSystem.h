#pragma once

#include <Core/World.h>

class IDebuggingSystem
{
public :
	virtual void OnDebugUpdate(World* world, float deltaTime) = 0;
};