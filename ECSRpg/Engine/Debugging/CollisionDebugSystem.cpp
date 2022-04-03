#include "CollisionDebugSystem.h"
#include <ImGui/imgui.h>

#include <Core/SceneTransformComponents.h>
#include <Physics/Collisions/CollisionComponents.h>

#include "DebugDraws.h"

void CollisionDebugSystem::OnDebugUpdate(World* world, float deltaTime)
{
	ImGui::Begin("Collision");

	ImGui::Checkbox("Collision Shape Rendering Enabled ", &renderColliders);

	ImGui::Text("Collision Shapes:");
	ImGui::Checkbox("Spheres", &renderSpheres);
	ImGui::Checkbox("AABBs", &renderAABBs);

	ImGui::End();

	if (renderColliders)
	{
		if (renderSpheres)
		{
			//Draw Spheres
			DrawSphereColliders(world);
		}
		if (renderAABBs)
		{
			//Draw AABBs
			DrawAABBColliders(world);
		}
	}
}

void CollisionDebugSystem::DrawAABBColliders(World* world)
{
	//ForEntities(world, Transform, AABBColliderComponent)
	//{
	//	Transform* transform = world->GetComponent<Transform>(entity);
	//	AABBColliderComponent* collider = world->GetComponent<AABBColliderComponent>(entity);

	//	CollisionAABB aabb = collider->GetAABBLimits(transform);


	//}
}

void CollisionDebugSystem::DrawSphereColliders(World* world)
{
	//ForEntities(world, Transform, SphereColliderComponent)
	//{
	//	Transform* transform = world->GetComponent<Transform>(entity);
	//	SphereColliderComponent* collider = world->GetComponent<SphereColliderComponent>(entity);

	//	CollisionSphere aabb = collider->GetCollisionSphere(transform);

	//	DebugDraws::DrawDebugSphere(aabb.Position, aabb.Radius + 0.02, 12);
	//}
}
