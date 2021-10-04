#include "BulletCreationHelpers.h"

#include <Core/World.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Maths/Vector3.h>
#include <Maths/Transform.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Components/BulletTags.h>

#include <GamePlay/Lifetime.h>

#include <Physics/Collisions/CollisionComponents.h>

#include <Data/NeOniCollisionLayers.h>


void BulletCreation::SpawnPlayerBullet(World* world, Vector3 position, Vector3 heading)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* meshAsset = resourceManager->GetMesh("SmoothSphere");
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	Entity bullet = world->CreateEntity();

	Transform* transform = world->AddComponent<Transform>(bullet);
	transform->SetPosition(position);
	transform->SetScale(Vector3(0.15f, 0.15f, 0.15f));

	MeshComponent* mesh = world->AddComponent<MeshComponent>(bullet);
	mesh->SetMesh(meshAsset);
	mesh->SetMaterial(testMaterial);

	world->AddComponent<PlayerBullet>(bullet);

	BulletComponent* bulletComp = world->AddComponent<BulletComponent>(bullet);
	bulletComp->damage = 20;

	BulletMovementComponent* bulletMovement = world->AddComponent<BulletMovementComponent>(bullet);
	bulletMovement->velocity = heading * 20.0f;

	LifetimeComponent* lifeTime = world->AddComponent<LifetimeComponent>(bullet);
	*lifeTime = LifetimeComponent(5.f, bullet);

	SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(bullet);
	new(sphere) SphereColliderComponent(0.15f);

	ColliderMetaComponent* colliderMeta = world->AddComponent< ColliderMetaComponent>(bullet);
	colliderMeta->type = ColliderType::Sphere;
	colliderMeta->isTrigger = true;

	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Default);
	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Enemies);

	colliderMeta->collisionLayer = NeOniCollisionLayers::PlayerBullets;

	world->AddComponent<DynamicCollider>(bullet);

}
