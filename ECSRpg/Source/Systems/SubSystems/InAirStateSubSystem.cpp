#include "InAirStateSubSystem.h"

#include <Core/World.h>


#include "../../Components/PlayerMovementComponent.h"
#include <Physics/PhysicsSystem.h>
#include <Physics/Collisions/CollisionDetection.h>


void CheckInAirState(World* world)
{
	std::vector<Entity> entities = world->GetEntities<PlayerMovementComponent, Transform>();
	PhysicsSystem* physicsSystem = world->GetPhysicsSystem();

	for (Entity entity : entities)
	{
		Transform* t = world->GetComponent<Transform>(entity);
		PlayerMovementComponent* p = world->GetComponent<PlayerMovementComponent>(entity);

		Vector3 playerPosition = t->GetPosition();

		Vector3 RayEnd = playerPosition + (VECTOR3_DOWN * 1.01);

		RaycastingResult result = physicsSystem->CastRay(playerPosition, RayEnd, entity);

		p->inAir = !result.hasHit;
	}
}
