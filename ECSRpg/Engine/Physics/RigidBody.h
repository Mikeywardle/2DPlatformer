#pragma once
#include <Maths/Vector3.h>

struct RigidBodyComponent
{
	bool isInfiniteMass;
	bool isAwake;
	//bool frictionAllowed = true;

	float mass = 0.0f;
	float Restitution = 0;
	float Friction = 0;

	Vector3 velocity = Vector3::Zero;

	float GetInverseMass();
	void AddForce(Vector3 force);
	void AddImpulse(Vector3 impulse);

	Vector3 GetResultantForce();
	Vector3 GetFrameForce();

	void ClearResultantForce();

	void AccumulateFriction(const Vector3& friction);
	void AccumulateDisplacement(const Vector3& displacement);

private:

	Vector3 resultantForce = Vector3::Zero;
	Vector3 frameForce = Vector3::Zero;

	Vector3 accumulatedFriction = Vector3::Zero;
	Vector3 accumulatedDisplacement = Vector3::Zero;

	
	static const float sleepThreshold;
};
