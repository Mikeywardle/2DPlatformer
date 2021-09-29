#pragma once

#include <ecs/System.h>
#include <Maths/Vector3.h>

class World;

struct SpringArmComponent
{
	SpringArmComponent() = default;
	SpringArmComponent(float InDistance, float InLerpSpeed, Entity InChild, Entity InParent);

	float distance;
	float lerpSpeed;
	Entity child;
	Entity parent;
};

class SpringArmSystem : public System
{
public:

	SpringArmSystem(World* world);

	virtual void OnFrame(float deltaTime) override;


};
