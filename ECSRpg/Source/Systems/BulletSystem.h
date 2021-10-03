#pragma once

#include <ecs/System.h>

struct TriggerOverlapEvent;

class BulletSystem final : public System
{
public:
	BulletSystem(World* world);

	virtual void OnFrame(const float deltaTime) override;

private:

	void OnBulletOverlap(TriggerOverlapEvent& triggerOverlaps);
};