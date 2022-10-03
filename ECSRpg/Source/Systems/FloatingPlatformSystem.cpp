#include "FloatingPlatformSystem.h"

#include <Core/World.h>

#include <Components/FloatingPlatformComponent.h>
#include <Physics/RigidBody.h>
#include <Core/SceneTransformComponents.h>

FloatingPlatformSystem::FloatingPlatformSystem(World* world)
	:System(world)
{
}

void FloatingPlatformSystem::OnFrame(const float deltaTime)
{
	world->ForEntities<FloatingPlatformComponent, RigidBodyComponent, SceneTransformComponent>
		(
			[](const Entity entity, FloatingPlatformComponent* fpc, RigidBodyComponent* rb, SceneTransformComponent* transform)
			{
				//Check need to change direction
				const Vector3 platformPosition = transform->GetPosition();
				const Vector3 target = fpc->towardsA ? fpc->a : fpc->b;
				const Vector3 origin = fpc->towardsA ? fpc->b : fpc->a;

				const float diffLength = Vector3::Magnitude(platformPosition - origin);
				const float fpcLength = Vector3::Magnitude(target - origin);

				if (diffLength >= fpcLength)
				{
					fpc->towardsA = !fpc->towardsA;
				}

				//Add velocity
				const Vector3 moveDir = !fpc->towardsA ? Vector3::Normalize(fpc->b - fpc->a) : Vector3::Normalize(fpc->a - fpc->b);
				rb->velocity = moveDir * fpc->moveSpeed;

			}
	);
}
