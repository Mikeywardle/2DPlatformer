#include "Gravity.h"
#include "../RigidBody.h"


void GravityForceHandler::ApplyForceForFrame(World* world, float deltaTime) const
{
	world->ForEntities<GravityComponent, RigidBodyComponent>
		(
			[&](const Entity entity, GravityComponent* gravComponent, RigidBodyComponent* rb)
			{
				Vector3 GravityForce = gravComponent->GravityScale * GravityValue * rb->mass;
				rb->AddForce(GravityForce);
			}

		);
}
