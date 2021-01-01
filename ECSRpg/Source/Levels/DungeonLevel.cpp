#include "DungeonLevel.h"

#include <Core/World.h>
#include <Core/Transform.h>

#include <Camera/CameraComponent.h>

#include <Debugging/FloatingCamera.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/Sprites/Sprite.h>
#include <Rendering/Sprites/BillBoardSprites.h>

#include <Physics/RigidBody.h>
#include <Physics/Collisions/AABB.h>
#include <Physics/Collisions/SphereCollider.h>

#include <Physics/Forces/AirResistance.h>
#include <Physics/Forces/Gravity.h>

#include "../Components/PlayerMovementComponent.h"

#include "../Systems/PlayerMovementSystem.h"

#include <string>

float tilewidth = 1.f;
void DungeonLevel::OnStart()
{

	SpawnPlayer();
	CreatePlayerCamera();


	CreateTile(Vector3(5, 0, 5), Vector3(5, .2, 5), "TestFloor");

	CreateTile(Vector3(9, .5, 2), Vector3(.5, .5, 1), "TestFloor");
	CreateTile(Vector3(8, .5, 6), Vector3(1, .5, 1), "TestFloor");

	CreateTile(Vector3(5, 2, 0), Vector3(5, 2, .2), "TestWall");
	CreateTile(Vector3(5, 2, 10), Vector3(5, 2, .2), "TestWall");

	CreateTile(Vector3(0, 2, 5), Vector3(.2, 2, 5), "TestWall");
	CreateTile(Vector3(10, 2, 5), Vector3(.2, 2, 5), "TestWall");



	//world->RegisterSystem<FloatingCameraMovementSystem>();

	world->RegisterSystem<BillBoardSpriteSystem>();
	world->RegisterSystem<PlayerMovementSystem>();
}

void DungeonLevel::SpawnPlayer()
{
	ResourceManager* resourceManager = world->GetResourceManager();
	Texture2D* spriteTexture = resourceManager->GetTexture("PlayerShip");

	Entity player = world->CreateEntity();

	SpriteComponent* renderer = world->AddComponent<SpriteComponent>(player);
	*renderer = SpriteComponent(spriteTexture, COLOR_WHITE, 64);

	Transform* t = world->AddComponent<Transform>(player);
	t->SetPosition(Vector3(5, 5, 5));
	t->SetRotation(Vector3(0, 0, 0));
	t->SetScale(VECTOR3_ONE);

	SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(player);
	new(sphere) SphereColliderComponent(.5);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
	rb->mass = 10;
	rb->Restitution = .01f;
	rb->Friction = 0.5f;

	GravityComponent* g = world->AddComponent<GravityComponent>(player);
	g->GravityScale = 1.f;

	AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
	a->DragCoefficient = 1.f;

	BillBoardComponent* billboard = world->AddComponent<BillBoardComponent>(player);
	*billboard = BillBoardComponent(false, true, false);

	PlayerMovementComponent* p = world->AddComponent<PlayerMovementComponent>(player);
	new(p) PlayerMovementComponent(50, .2, .1);

}

void DungeonLevel::CreatePlayerCamera()
{
	//Spawn Camera
	Entity cameraEntity = world->CreateEntity();

	CameraComponent* c = world->AddComponent<CameraComponent>(cameraEntity);

	c->fov = 45.f;
	c->farPlane = 1000.f;
	c->nearPlane = 0.1f;
	c->projectionType = ProjectionType::PERSPECTIVE;

	Transform* t = world->AddComponent<Transform>(cameraEntity);
	t->SetPosition(Vector3(-2, 12, -2));
	t->SetRotation(Vector3(-45, 45, -45));

	CameraComponent::SetMainCamera(cameraEntity);

	FloatingCameraComponent* fc = world->AddComponent<FloatingCameraComponent>(cameraEntity);
	*fc = FloatingCameraComponent(5.f, .1f);

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
	rb->Friction = 1.2;

	AABBColliderComponent* sc = world->AddComponent<AABBColliderComponent>(platform);
	new(sc) AABBColliderComponent(Scale.x, Scale.y, Scale.z);
}
