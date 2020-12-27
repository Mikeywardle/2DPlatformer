#pragma once
#include <Maths/Vector3.h>

struct RigidBodyComponent
{
	bool isInfiniteMass;
	bool isAwake;
	float mass;
	float Restitution = 0;
	float Friction = 0;

	Vector3 velocity = VECTOR3_ZERO;

	float GetInverseMass();
	void AddForce(Vector3 force);
	void AddImpulse(Vector3 impulse);

	Vector3 GetResultantForce();
	Vector3 GetFrameForce();

	void ClearResultantForce();

private:

	Vector3 resultantForce = VECTOR3_ZERO;
	Vector3 frameForce = VECTOR3_ZERO;


};
