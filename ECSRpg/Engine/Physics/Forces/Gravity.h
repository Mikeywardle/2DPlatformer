#pragma once
#include "../ForceHandler.h"

struct GravityComponent
{
	float GravityScale = 1;
};

class GravityForceHandler final : public IForceHandler
{
	const float GravityValue = 9.81f;

	virtual void ApplyForceForFrame(World* world,float deltaTime) const;
};
