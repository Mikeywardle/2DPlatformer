#pragma once
#include <Core/World.h>

class IForceHandler
{
public:

	virtual void ApplyForceForFrame(World* world, float deltaTime) const = 0;
};
