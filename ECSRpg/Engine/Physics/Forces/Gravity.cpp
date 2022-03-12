#include "Gravity.h"
#include "../RigidBody.h"


void GravityForceHandler::ApplyForceForFrame(World* world, float deltaTime) const
{
	ForEntities(world, GravityComponent, RigidBodyComponent)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		GravityComponent* gravComponent = world->GetComponent<GravityComponent>(entity);

		Vector3 GravityForce = Vector3::Down * (gravComponent->GravityScale * GravityValue) * rb->mass;
		rb->AddForce(GravityForce);
	}
}
