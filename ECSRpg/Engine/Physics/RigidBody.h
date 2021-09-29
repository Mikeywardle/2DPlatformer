#pragma once
#include <Maths/Vector3.h>

struct RigidBodyComponent
{
	bool isInfiniteMass;
	bool isAwake;
	float mass;
	float Restitution = 0;
	float Friction = 0;

	Vector3 velocity = Vector3::Zero;

	float GetInverseMass();
	void AddForce(Vector3 force);
	void AddImpulse(Vector3 impulse);

	Vector3 GetResultantForce();
	Vector3 GetFrameForce();

	void ClearResultantForce();

private:

	Vector3 resultantForce = Vector3::Zero;
	Vector3 frameForce = Vector3::Zero;


};
