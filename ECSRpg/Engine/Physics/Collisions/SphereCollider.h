#pragma once
#include <Maths/Vector3.h>
#include <ecs/Entity.h>
#include "Collider.h"

struct SphereColliderComponent
{
	SphereColliderComponent() = default;
	SphereColliderComponent(float radius);
	float radius;
};


struct CollisionSphere
{
	CollisionSphere() = default;
	CollisionSphere(Entity entity, float Radius, Vector3 Position);

	Entity entity;
	float Radius;
	Vector3 Position;
};

CollisionResult TestSpherevsSphere(CollisionSphere& A, CollisionSphere& B);