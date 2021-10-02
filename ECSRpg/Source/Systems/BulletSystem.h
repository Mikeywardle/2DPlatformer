#pragma once

#include <ecs/System.h>

class BulletSystem final : public System
{
public:
	BulletSystem(World* world);

	virtual void OnFrame(const float deltaTime) override;
};