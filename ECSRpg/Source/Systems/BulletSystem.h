#pragma once

#include <ecs/System.h>

struct TriggerOverlapEvent;
struct PhysicsCollisionResult;

typedef unsigned int Entity;

class BulletSystem final : public System
{
public:
	BulletSystem(World* world);

	virtual void OnFrame(const float deltaTime) override;

private:

	void OnBulletOverlap(TriggerOverlapEvent& triggerOverlaps);

	bool CheckEntityIsBullet(PhysicsCollisionResult result, Entity& bulletEntity, Entity& targetEntity) const;
	bool CheckBulletAndTargetCompatible(const Entity bulletEntity, const Entity targetEntity) const;
};