#include "RigidBody.h"

float RigidBodyComponent::GetInverseMass()
{
    if(isInfiniteMass)
        return 0;
    else
        return 1.f/mass;
}

void RigidBodyComponent::AddForce(Vector3 force)
{
    resultantForce += force;
}

void RigidBodyComponent::AddImpulse(Vector3 impulse)
{
    velocity += impulse * GetInverseMass();
}

Vector3 RigidBodyComponent::GetResultantForce()
{
    return resultantForce;
}

Vector3 RigidBodyComponent::GetFrameForce()
{
    return frameForce;
}

void RigidBodyComponent::ClearResultantForce()
{
    frameForce = resultantForce;
    resultantForce = Vector3::Zero;

    accumulatedFriction = Vector3::Zero;
    accumulatedDisplacement = Vector3::Zero;
}

void RigidBodyComponent::AccumulateFriction(const Vector3& friction)
{
}

void RigidBodyComponent::AccumulateDisplacement(const Vector3& displacement)
{
}
