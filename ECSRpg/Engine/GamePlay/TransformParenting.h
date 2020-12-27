#pragma once
#include <ecs/Entity.h>
#include <Maths/Vector3.h>
#include <ecs/System.h>

struct PositionAttatchmentComponent
{
	PositionAttatchmentComponent() = default;
	PositionAttatchmentComponent(Entity Parent, Vector3 LocalOffset = VECTOR3_ZERO, bool DestroyOnNoParent = true);

	bool DestroyOnNoParent;
	Entity Parent;
	Vector3 LocalOffset;
};

class PositionAttachmentSystem : public System
{
public:
	PositionAttachmentSystem(World* world) : System(world) {};

protected:

	virtual void OnFrame(float deltaTime) override;

};