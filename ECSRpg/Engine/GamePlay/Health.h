#pragma once
#include <ecs/System.h>

struct HealthComponent
{
	HealthComponent() = default;
	HealthComponent(short Health, bool canBeInvincible = false, float InvincibilityTime = 0);

	bool isInvincible = false;
	bool canBeInvincible;
	short Health;
	float InvincibilityTime;
	float remainingInvincibilityTime;


	void DealDamage(short Damage);
};

class InvincibilityTimingSystem : public System
{
public:
	InvincibilityTimingSystem(World* world) : System(world) {}

	virtual void OnFrame(float deltaTime);
};

class NoHealthCleanupSystem : public System
{
public:
	NoHealthCleanupSystem(World* world) : System(world) {}

	virtual void OnFrame(float deltaTime);
};