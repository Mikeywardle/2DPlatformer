#pragma once

#include "../ForceHandler.h"

struct AirResistanceComponent
{
	float DragCoefficient;
};

class AirResistanceForceHandler : public IForceHandler
{
	virtual void ApplyForceForFrame(World* world, float deltaTime);
};