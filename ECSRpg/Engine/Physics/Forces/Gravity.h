#pragma once
#include "../ForceHandler.h"

struct GravityComponent
{
	float GravityScale = 1;
};

class GravityForceHandler : public IForceHandler
{
	const float GravityValue = 9.81f;

	virtual void ApplyForceForFrame(World* world,float deltaTime);
};
