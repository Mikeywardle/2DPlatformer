#pragma once

#include <ecs/System.h>
#include <ecs/Entity.h>

struct LifetimeComponent
{
	LifetimeComponent() = default;
	LifetimeComponent(float lifetime, Entity entity, bool doesDecay = true);

	bool doesDecay;
	Entity entity;
	float lifeRemaining;
};


class LifeTimeDecaySystem : public System
{
public:
	LifeTimeDecaySystem(World* world) : System(world) {};

	virtual void OnFrame(float deltaTime);
};