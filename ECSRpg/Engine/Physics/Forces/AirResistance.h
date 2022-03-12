#pragma once

#include "../ForceHandler.h"

struct AirResistanceComponent
{
	float DragCoefficient;
};

class AirResistanceForceHandler final : public IForceHandler
{
	virtual void ApplyForceForFrame(World* world, float deltaTime) const;
};