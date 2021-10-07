#include "EnemyCreationHelpers.h"

#include <Core/World.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Physics/Forces/AirResistance.h>
#include <Physics/Forces/Gravity.h>
#include <Physics/RigidBody.h>
#include <Physics/Collisions/CollisionComponents.h>

#include <Components/EnemyComponents.h>
#include <GamePlay/Health.h>

#include <Data/NeOniCollisionLayers.h>


void EnemyCreation::SpawnEnemy(World* world, Vector3 position)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* meshAsset = resourceManager->GetMesh("SmoothSphere");
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	//Create Player
	Entity enemy = world->CreateEntity();

	Transform* t = world->AddComponent<Transform>(enemy);
	t->SetPosition(position);
	t->SetRotation(Vector3(0, 0, 0));
	t->SetScale(Vector3(1, 1, 1));

	MeshComponent* m = world->AddComponent<MeshComponent>(enemy);
	m->SetMesh(meshAsset);
	m->SetMaterial(testMaterial);

	SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(enemy);
	new(sphere) SphereColliderComponent(1);

	ColliderMetaComponent* colliderMeta = world->AddComponent< ColliderMetaComponent>(enemy);
	colliderMeta->type = ColliderType::Sphere;

	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Default);
	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::PlayerBullets);
	colliderMeta->toCollideLayers.push_back(NeOniCollisionLayers::Enemies);

	colliderMeta->collisionLayer = NeOniCollisionLayers::Enemies;

	world->AddComponent<DynamicCollider>(enemy);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(enemy);
	rb->mass = 10;
	rb->Restitution = .01f;
	rb->Friction = 1.f;

	GravityComponent* g = world->AddComponent<GravityComponent>(enemy);
	g->GravityScale = 1.f;

	AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(enemy);
	a->DragCoefficient = 1.f;

	HealthComponent* health = world->AddComponent<HealthComponent>(enemy);
	*health = HealthComponent(100);

	world->AddComponent<EnemyTag>(enemy);
}
