#include "AirResistance.h"

#include "../RigidBody.h"

void AirResistanceForceHandler::ApplyForceForFrame(World* world, float deltaTime) const
{
	ForEntities(world, AirResistanceComponent, RigidBodyComponent)
	{
		RigidBodyComponent* rb = world->GetComponent<RigidBodyComponent>(entity);
		AirResistanceComponent* arComponent = world->GetComponent<AirResistanceComponent>(entity);

		Vector3 velocity = rb->velocity;

		float Speed = Vector3::Magnitude(velocity);
		Vector3 VelocityDir = Vector3::Normalize(velocity);

		rb->AddForce(VelocityDir * (Speed * Speed * arComponent->DragCoefficient * -1));
	}
}
