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
#include <Components/WeaponComponents.h>

#include <Camera/CameraComponent.h>

#include <GamePlay/TransformParenting.h>

#include <Data/NeOniCollisionLayers.h>

namespace PlayerCreation
{
void SpawnPlayer(World* world)
	{
		ResourceManager* resourceManager = world->GetResourceManager();

		Mesh* meshAsset = resourceManager->GetMesh("SmoothSphere");
		Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

		//Create Player
		Entity player = world->CreateEntity();

		Transform* t = world->AddComponent<Transform>(player);
		t->SetPosition(Vector3(5, 2, 5));
		t->SetRotation(Vector3(0, 0, 0));
		t->SetScale(Vector3(0.5f, 0.5f, 0.5f));

		//MeshComponent* m = world->AddComponent<MeshComponent>(player);
		//m->SetMesh(meshAsset);
		//m->SetMaterial(testMaterial);

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

		PlayerWeaponComponent* weapon = world->AddComponent<PlayerWeaponComponent>(camera);
		*weapon = PlayerWeaponComponent(0.1f);
	}
}
