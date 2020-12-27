#include "Gravity.h"
#include "../RigidBody.h"


void GravityForceHandler::ApplyForceForFrame(World* world, float deltaTime)
{
	std::vector<Entity> entities = world->GetEntities<GravityComponent, RigidBodyComponent>();

	for (Entity entity : entities)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		GravityComponent* gravComponent = world->GetComponent<GravityComponent>(entity);

		Vector3 GravityForce = VECTOR3_DOWN * (gravComponent->GravityScale * GravityValue) * rb->mass;
		rb->AddForce(GravityForce);
	}
}
