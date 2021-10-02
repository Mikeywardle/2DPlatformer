#include "DungeonLevel.h"

#include <Core/World.h>
#include <Maths/Transform.h>

#include <Camera/CameraComponent.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Lighting/LightSources/DirectionalLight.h>

#include <Physics/RigidBody.h>
#include <Physics/Collisions/CollisionComponents.h>


#include <Systems/PlayerMovementSystem.h>
#include <Systems/BulletSystem.h>
#include <Systems/PlayerWeaponSystem.h>

#include <Helpers/PlayerCreationHelpers.h>

#include <GamePlay/TransformParenting.h>
#include <GamePlay/Lifetime.h>



void DungeonLevel::LoadLevel()
{
	PlayerCreation::SpawnPlayer(world);

	CreateDirectionalLight(Vector3(80, -10, 30));

	//Floor
	CreateTile(Vector3(5, 0, 5), Vector3(10, .2, 10), "TestFloor");

	//Blocks
	CreateTile(Vector3(9, .7, 2), Vector3(.5, .5, 1), "TestMaterial");
	CreateTile(Vector3(8, .7, 6), Vector3(1, .5, 1), "TestMaterial");

	CreateTile(Vector3(5, 2, -5), Vector3(10, 2, .2), "TestWall");
	CreateTile(Vector3(5, 2, 15), Vector3(10, 2, .2), "TestWall");

	CreateTile(Vector3(-5, 2, 5), Vector3(.2, 2, 10), "TestWall");
	CreateTile(Vector3(15, 2, 5), Vector3(.2, 2, 10), "TestWall");

	//Steps
	//CreateTile(Vector3(5, .2, 9), Vector3(2, .05, 5), "TestFloor");

	//CreateMesh(Vector3(5, 5, 5), "SmoothSphere");
	//CreateMesh(Vector3(8, 5, 8), "SmoothSphere");
}

void DungeonLevel::OnStart()
{
	//Player Systems
	world->RegisterSystem<PlayerMovementSystem>();
	world->RegisterSystem<PlayerWeaponSystem>();

	//Bullet Systems
	world->RegisterSystem<BulletSystem>();

	//Gameplay Utils
	world->RegisterSystem<PositionAttachmentSystem>();

	world->RegisterSystem<LifeTimeDecaySystem>();
}

void DungeonLevel::CreatePlayerCamera()
{

}

void DungeonLevel::CreateTile(Vector3 Position, Vector3 Scale, std::string materialName)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* cubeMeshAsset = resourceManager->GetMesh("Cube");
	Material* testMaterial = resourceManager->GetMaterial(materialName);

	//Spawn Mesh
	Entity platform = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(platform);
	m->SetMesh(cubeMeshAsset);
	m->SetMaterial(testMaterial);

	Transform* t4 = world->AddComponent<Transform>(platform);
	t4->SetPosition(Position);
	t4->SetScale(Scale);
	t4->SetStatic(true);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(platform);
	rb->isInfiniteMass = true;
	rb->Friction = 3.f;

	AABBColliderComponent* sc = world->AddComponent<AABBColliderComponent>(platform);
	new(sc) AABBColliderComponent(Scale);

	ColliderMetaComponent* colliderMeta = world->AddComponent< ColliderMetaComponent>(platform);
	colliderMeta->type = ColliderType::AABB;

	world->AddComponent<StaticCollider>(platform);
}

void DungeonLevel::CreateDirectionalLight(Vector3 rotation)
{

	Entity light = world->CreateEntity();

	DirectionalLightComponent* dl = world->AddComponent<DirectionalLightComponent>(light);
	*dl = DirectionalLightComponent(COLOR_WHITE, .05);

	Transform* t = world->AddComponent<Transform>(light);
	t->SetRotation(rotation);

}

void DungeonLevel::CreateMesh(Vector3 position, std::string mesh)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* meshAsset = resourceManager->GetMesh(mesh);
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	//Spawn Mesh
	Entity platform = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(platform);
	m->SetMesh(meshAsset);
	m->SetMaterial(testMaterial);

	Transform* t = world->AddComponent<Transform>(platform);
	t->SetPosition(position);
	t->SetStatic(true);
	t->SetScale(Vector3::One);
}
