#include "AirResistance.h"

#include "../RigidBody.h"

void AirResistanceForceHandler::ApplyForceForFrame(World* world, float deltaTime) const
{
	world->ForEntities<RigidBodyComponent, AirResistanceComponent>
		(
			[&](const Entity entity, RigidBodyComponent* rb, AirResistanceComponent* arComponent)
			{
				Vector3 velocity = rb->velocity;

				float Speed = Vector3::Magnitude(velocity);
				Vector3 VelocityDir = Vector3::Normalize(velocity);

				rb->AddForce(VelocityDir * (Speed * Speed * arComponent->DragCoefficient * -1));
			}
		);
}
