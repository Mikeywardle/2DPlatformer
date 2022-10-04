#include "PlayerCreationHelpers.h"

#include <Core/World.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/RenderDataComponents.h>

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

		SceneTransformComponent* t = world->AddComponent<SceneTransformComponent>(player);
		t->SetPosition(Vector3(0, 2, 5));
		t->SetRotation(Vector3(0, 90, 0));

		RenderScale* renderScale = world->AddComponent<RenderScale>(player);
		renderScale->scale = Vector3(0.5f, 0.5f, 0.5f);

		SphereCollisionGeometry* geometry = new SphereCollisionGeometry();
		geometry->radius = 0.5f;

		ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(player);

		collider->toCollideLayers.push_back(NeOniCollisionLayers::Default);
		collider->toCollideLayers.push_back(NeOniCollisionLayers::Environment);

		collider->collisionLayer = NeOniCollisionLayers::Players;

		collider->SetCollisionGeometry(geometry);

		world->AddComponent<DynamicCollider>(player);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
		rb->mass = 10;
		rb->Restitution = .01f;
		rb->Friction = 1.f;

		GravityComponent* g = world->AddComponent<GravityComponent>(player);
		//g->GravityScale = Vector3::Down;

		AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
		a->DragCoefficient = 1.f;


		world->AddComponent<CurrentPossesedPlayer>(player);
		world->AddComponent<PlayerTag>(player);

		//Create Camera
		Entity camera = world->CreateEntity();

		PlayerMovementComponent* pmc = world->AddComponent<PlayerMovementComponent>(player);
		*pmc = PlayerMovementComponent
		(
			10.0f
			, 2.f
			, 100.f
			, 1
		);

		pmc->maxWalkAngle = 30;

		CameraComponent* c = world->AddComponent<CameraComponent>(camera);

		c->fov = 45.f;
		c->farPlane = 1000.f;
		c->nearPlane = 0.1f;
		c->projectionType = ProjectionType::PERSPECTIVE;

		SceneTransformComponent* cameraTransform = world->AddComponent<SceneTransformComponent>(camera);
		cameraTransform->SetLocalPosition(Vector3(0, 0.5, 0));

		SceneTransformComponent* playerTransform = world->GetComponent<SceneTransformComponent>(player);
		cameraTransform = world->GetComponent<SceneTransformComponent>(camera);

		playerTransform->AttachChild(cameraTransform);

		PlayerMovementCamera* cameramovement = world->AddComponent<PlayerMovementCamera>(camera);
		cameramovement->RotationSpeed = 1000.f;

		pmc->cameraEntity = camera;

		CameraComponent::SetMainCamera(camera);


	}

	void SpawnDungeonPlayer(World* world, const Vector3& position)
	{
		//Create Player
		Entity player = world->CreateEntity();

		SceneTransformComponent* t = world->AddComponent<SceneTransformComponent>(player);
		t->SetPosition(position);
		t->SetRotation(Vector3(0, 0, 0));

		RenderScale* renderScale = world->AddComponent<RenderScale>(player);
		renderScale->scale = Vector3(0.5f, 0.5f, 0.5f);

		AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
		geometry->HalfLimits = Vector3(0.5f, 1.0f, 0.5f);

		ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(player);

		collider->toCollideLayers.push_back(NeOniCollisionLayers::Default);
		collider->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
		collider->toCollideLayers.push_back(NeOniCollisionLayers::Floors);

		collider->collisionLayer = NeOniCollisionLayers::Players;

		collider->SetCollisionGeometry(geometry);

		world->AddComponent<DynamicCollider>(player);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
		rb->mass = 10;
		rb->Restitution = .01f;
		rb->Friction = 1.f;

		GravityComponent* g = world->AddComponent<GravityComponent>(player);
		//g->GravityScale = 1.f;

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

		world->AddComponent<SceneTransformComponent>(camera);

		CameraComponent::SetMainCamera(camera);

		PositionAttatchmentComponent* pac = world->AddComponent<PositionAttatchmentComponent>(camera);
		*pac = PositionAttatchmentComponent(player, Vector3(0, 0.f, 0));
	}
}
