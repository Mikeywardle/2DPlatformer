#pragma once

#include <Core/World.h>
#include <Collisions/CollisionDetection.h>
#include <Physics/Collisions/CollisionComponents.h>

class World;

class PhysicsWorld;
struct PhysicsCollisionResult;

struct ColliderMetaComponent;
struct IColliderComponent;
struct RigidBodyComponent;
struct Vector3;

typedef unsigned int Entity;

class CollisionSystem
{
public:
	CollisionSystem(World* world);

	void ProcessCollisions(PhysicsWorld* physWorld);

	void GenerateStaticColiisions(PhysicsWorld* physWorld);

#pragma region Shape Casting

	template<typename ...Args>
	RaycastingResult CastRay(Vector3 Start, Vector3 End, Entity ignoreEntity)
	{
		std::vector<RaycastingResult> raycastResults;

		Ray ray = Ray(Start, End);
		QuerySpheresWithRay<Args...>(ray, raycastResults, ignoreEntity);
		QueryAABBsWithRay<Args...>(ray, raycastResults, ignoreEntity);

		if (raycastResults.size() == 0)
		{
			return RaycastingResult();
		}
		else 
		{
			RaycastingResult smallestResult = raycastResults[0];

			for (int i = 1; i < raycastResults.size(); ++i)
			{
				RaycastingResult result = raycastResults[i];

				if (result.Distance < smallestResult.Distance)
					smallestResult = result;
			}

			return smallestResult;
		}
	}
#pragma endregion

private:

	void FindDynamicCollisions(std::vector<PhysicsCollisionResult>& colliderResults, std::vector<PhysicsCollisionResult>& triggerResults, PhysicsWorld* physWorld);
	IColliderComponent* GetEntityCollider(const Entity entity, const ColliderMetaComponent* metaData);

	void ResolveCollisions(std::vector<PhysicsCollisionResult>& results);

	void AddImpulseIfNotNull(RigidBodyComponent* rba, const Vector3& frictionImpulse);
	Vector3 GetRelativeVelocity(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetSeparatingVelocity(Vector3 RelativeVelocity, Vector3 Normal);
	float GetCollisionRestitution(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalInversMass(RigidBodyComponent* rb1, RigidBodyComponent* rb2);
	float GetTotalFrictionCoefficient(RigidBodyComponent* rb1, RigidBodyComponent* rb2);

#pragma region Templated Queries
	template<typename ...Args>
	void QuerySpheresWithRay(Ray& ray,
		std::vector<RaycastingResult>& rayCastingResults,
		Entity ignoreEntity = NO_ENTITY
	) const
	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., SphereColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			SphereColliderComponent* collider = world->GetComponent<SphereColliderComponent>(entity);

			CollisionSphere sphere = CollisionSphere(collider->radius, transform->GetPosition());

			RaycastingResult result = TestRayVsSphere(ray, sphere);

			if (result.hasHit)
				rayCastingResults.push_back(result);
		}
	}

	template<typename ...Args>
	void QueryAABBsWithRay(Ray& ray,
		std::vector<RaycastingResult>& rayCastingResults,
		Entity ignoreEntity = NO_ENTITY
	) const
	{
		std::vector<Entity> Colliders = world->GetEntities<Args..., AABBColliderComponent, Transform>();

		for (Entity entity : Colliders)
		{
			if (entity == ignoreEntity)
				continue;

			Transform* transform = world->GetComponent<Transform>(entity);
			AABBColliderComponent* collider = world->GetComponent<AABBColliderComponent>(entity);


			CollisionAABB box = CollisionAABB(
				transform->GetPosition(),
				collider->HalfLimits);

			RaycastingResult result = TestRayVsAABB(ray, box);

			if (result.hasHit)
				rayCastingResults.push_back(result);
		}
	}

#pragma endregion


	World* world;
};
