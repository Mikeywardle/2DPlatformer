#include "PlayerCreationHelpers.h"

#include <Core/World.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Physics/Forces/AirResistance.h>
#include <Physics/Forces/Gravity.h>
#include <Physics/RigidBody.h>
#include <Physics/Collisions/CollisionComponents.h>

#include <Components/PlayerMovementComponent.h>
#include <Components/PlayerTags.h>

#include <Camera/CameraComponent.h>

#include <GamePlay/TransformParenting.h>

#include <Data/NeOniCollisionLayers.h>

namespace PlayerCreation
{
	void SpawnPlayer(World* world, const Vector3& position)
	{
		//Create Player
		Entity player = world->CreateEntity();

		Transform* t = world->AddComponent<Transform>(player);
		t->SetPosition(Vector3(0, 2, 5));
		t->SetRotation(Vector3(0, 0, 0));
		t->SetScale(Vector3(0.5f, 0.5f, 0.5f));

		SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(player);
		new(sphere) SphereColliderComponent(.5);

		ColliderMetaComponent* colliderMeta = world->AddComponent< ColliderMetaComponent>(player);
		colliderMeta->type = ColliderType::Sphere;

		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Default);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Enemies);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::EnemyBullets);

		colliderMeta->collisionLayer = NeOniCollisionLayers::Players;

		world->AddComponent<DynamicCollider>(player);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
		rb->mass = 10;
		rb->Restitution = .01f;
		rb->Friction = 1.f;

		GravityComponent* g = world->AddComponent<GravityComponent>(player);
		g->GravityScale = 1.f;

		AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
		a->DragCoefficient = 1.f;

		PlayerMovementComponent* pmc = world->AddComponent<PlayerMovementComponent>(player);
		*pmc = PlayerMovementComponent
		(
			10.0f
			, 2.f
			, 100.f
			, 2
			, 1000.f
		);

		world->AddComponent<CurrentPossesedPlayer>(player);
		world->AddComponent<PlayerTag>(player);

		//Create Camera
		Entity camera = world->CreateEntity();

		CameraComponent* c = world->AddComponent<CameraComponent>(camera);

		c->fov = 45.f;
		c->farPlane = 1000.f;
		c->nearPlane = 0.1f;
		c->projectionType = ProjectionType::PERSPECTIVE;

		world->AddComponent<Transform>(camera);

		CameraComponent::SetMainCamera(camera);

		PositionAttatchmentComponent* pac = world->AddComponent<PositionAttatchmentComponent>(camera);
		*pac = PositionAttatchmentComponent(player, Vector3(0,1,0));
	}

	void SpawnDungeonPlayer(World* world, const Vector3& position)
	{
		//Create Player
		Entity player = world->CreateEntity();

		Transform* t = world->AddComponent<Transform>(player);
		t->SetPosition(position);
		t->SetRotation(Vector3(0, 0, 0));
		t->SetScale(Vector3(0.5f, 0.5f, 0.5f));

		AABBColliderComponent* collider = world->AddComponent<AABBColliderComponent>(player);
		collider->HalfLimits = Vector3(0.5f, 1.0f, 0.5f);

		ColliderMetaComponent* colliderMeta = world->AddComponent< ColliderMetaComponent>(player);
		colliderMeta->type = ColliderType::AABB;

		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Default);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Enemies);
		colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::EnemyBullets);

		colliderMeta->collisionLayer = NeOniCollisionLayers::Players;

		world->AddComponent<DynamicCollider>(player);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
		rb->mass = 10;
		rb->Restitution = .01f;
		rb->Friction = 1.f;

		GravityComponent* g = world->AddComponent<GravityComponent>(player);
		g->GravityScale = 1.f;

		world->AddComponent<CurrentPossesedPlayer>(player);
		world->AddComponent<PlayerTag>(player);

		world->AddComponent<DungeonPlayerMovementComponent>(player);

		//Create Camera
		Entity camera = world->CreateEntity();

		CameraComponent* c = world->AddComponent<CameraComponent>(camera);

		c->fov = 45.f;
		c->farPlane = 1000.f;
		c->nearPlane = 0.1f;
		c->projectionType = ProjectionType::PERSPECTIVE;

		world->AddComponent<Transform>(camera);

		CameraComponent::SetMainCamera(camera);

		PositionAttatchmentComponent* pac = world->AddComponent<PositionAttatchmentComponent>(camera);
		*pac = PositionAttatchmentComponent(player, Vector3(0, 0.f, 0));
	}
}
