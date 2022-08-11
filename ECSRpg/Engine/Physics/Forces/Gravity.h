#pragma once
#include "../ForceHandler.h"

#include <Maths/Vector3.h>

struct GravityComponent
{
	Vector3 GravityScale = Vector3::Down;
};

class GravityForceHandler final : public IForceHandler
{
	const float GravityValue = 9.81f;

	virtual void ApplyForceForFrame(World* world,float deltaTime) const;
};
