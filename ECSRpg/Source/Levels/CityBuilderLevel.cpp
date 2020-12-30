#include "CityBuilderLevel.h"

#include <Core/World.h>
#include <Camera/CameraComponent.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/Sprites/Sprite.h>
#include <Resources/ResourceManager.h>

#include <Physics/Collisions/SphereCollider.h>
#include <Physics/Collisions/AABB.h>
#include <Physics/Forces/Gravity.h>
#include <Physics/Forces/AirResistance.h>
#include <Physics/RigidBody.h>

#include <Debugging/FloatingCamera.h>

#include <UI/Text.h>

#include <sstream>

#include "../Components/PlayerMovementComponent.h"
#include "../Components/CoinPickupComponent.h"
#include "../Components/FloatingPlatformComponent.h"
#include "../Components/RotatorComponent.h"

#include "../Systems/PlayerMovementSystem.h"
#include "../Systems/PlayerCameraSystem.h"
#include "../Systems/CoinSystem.h"
#include "../Systems/FloatingPlatformSystem.h"
#include "../Systems/RotatorSystem.h"

#include <ImGui/imgui.h>

struct TestBallTag {};

void CityBuilderLevel::OnStart()
{
	Level::OnStart();

	CreatePlayerCamera();
	CreatePlayer();

	CreatePlatform(Vector3(0, 0, 0), 30, 1);
	CreatePlatform(Vector3(32.5, -5, 0), 8, 1);
	CreatePlatform(Vector3(-30, 30, 0), 1, 30);
	CreatePlatform(Vector3(53, 2, 0), 15, 1);

	CreatePlatform(Vector3(75, -3, 0), 1, 1);
	CreatePlatform(Vector3(80, -3, 0), 1, 1);
	CreatePlatform(Vector3(85, -3, 0), 1, 1);
	CreatePlatform(Vector3(90, -3, 0), 1, 1);

	CreateFloatingPlatform(Vector3(72, 4, 0), 2.5, .5, Vector3(72, 4, 0), Vector3(92, 4, 0));

	CreatePlatform(Vector3(105, 2, 0), 5, 1);
	CreatePlatform(Vector3(120, 5, 0), 5, 1);

	CreateCoin(Vector3(0,2.5,0));
	CreateCoin(Vector3(4,2.5,0));
	CreateCoin(Vector3(8,2.5,0));

	CreateCoin(Vector3(40,4.5,0));
	CreateCoin(Vector3(44,4.5,0));
	CreateCoin(Vector3(48,4.5,0));

	CreateSprite(Vector3(0, 5, 0));

	//world->RegisterSystem<FloatingCameraMovementSystem>();

	world->RegisterSystem<PlayerMovementSystem>();
	world->RegisterSystem<PlayerCameraSystem>();

	world->RegisterSystem<CoinSystem>();
	world->RegisterSystem<FloatingPlatformSystem>();
	world->RegisterSystem<RotatorSystem>();
}

void CityBuilderLevel::OnFrame(float deltaTime)
{
}

void CityBuilderLevel::CreatePlayer()
{	
	ResourceManager* resourceManager = world->GetResourceManager();
	Mesh* sphereMeshAsset = resourceManager->GetMesh("Sphere");
	Texture2D* spriteTexture = resourceManager->GetTexture("PlayerShip");
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	Entity player = world->CreateEntity();

	MeshComponent *m = world->AddComponent<MeshComponent>(player);
	m->SetMesh(sphereMeshAsset);
	m->SetMaterial(testMaterial);

	//SpriteComponent* renderer = world->AddComponent<SpriteComponent>(player);
	//*renderer = SpriteComponent(spriteTexture, COLOR_WHITE, 32);

	Transform* t = world->AddComponent<Transform>(player);
	t->SetPosition(Vector3(-5, 5, 0));
	t->SetRotation(Vector3(0, 180, 0));
	t->SetScale(VECTOR3_ONE);

	SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(player);
	new(sphere) SphereColliderComponent(1);

	//AABBColliderComponent* box = world->AddComponent<AABBColliderComponent>(player);
	//new(box) AABBColliderComponent(1, 1, 1);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
	rb->mass = 10;
	rb->Restitution = .01f;
	rb->Friction = 0.8f;

	GravityComponent* g = world->AddComponent<GravityComponent>(player);
	g->GravityScale = 1.f;

	AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
	a->DragCoefficient = 1.f;

	PlayerMovementComponent* p = world->AddComponent<PlayerMovementComponent>(player);
	new(p) PlayerMovementComponent(2,100, .5,.3);

	world->AddComponent<TestBallTag>(player);
}

void CityBuilderLevel::CreateSprite(Vector3 Position)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Texture2D* spriteTexture = resourceManager->GetTexture("PlayerShip");

	Entity sprite = world->CreateEntity();

	Transform* t = world->AddComponent<Transform>(sprite);
	t->SetPosition(Position);
	t->SetRotation(Vector3(0, 180, 0));
	t->SetScale(VECTOR3_ONE);

	SpriteComponent* renderer = world->AddComponent<SpriteComponent>(sprite);
	*renderer = SpriteComponent(spriteTexture,COLOR_WHITE, 32);



}

Entity CityBuilderLevel::CreatePlatform(Vector3 position, float width, float height)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* cubeMeshAsset = resourceManager->GetMesh("Cube");
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	//Spawn Mesh
	Entity platform = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(platform);
	m->SetMesh(cubeMeshAsset);
	m->SetMaterial(testMaterial);

	Transform* t4 = world->AddComponent<Transform>(platform);
	t4->SetPosition(position);
	t4->SetScale(Vector3(width, height, 5));
	t4->SetStatic(true);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(platform);
	rb->isInfiniteMass = true;
	rb->Friction = 1.2;

	AABBColliderComponent* sc = world->AddComponent<AABBColliderComponent>(platform);
	new(sc) AABBColliderComponent(width, height, 5);

	return platform;
}

void CityBuilderLevel::CreateFloatingPlatform(Vector3 position, float width, float height, Vector3 start, Vector3 end)
{
	Entity platform = CreatePlatform(position, width, height);
	world->GetComponent<Transform>(platform)->SetStatic(false);

	FloatingPlatformComponent* f = world->AddComponent<FloatingPlatformComponent>(platform);
	*f = FloatingPlatformComponent(start, end, 2.f);
}

void CityBuilderLevel::CreateCoin(Vector3 position)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* MeshAsset = resourceManager->GetMesh("Cylinder");
	Material* testMaterial = resourceManager->GetMaterial("CoinMaterial");

	//Spawn Mesh
	Entity coin = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(coin);
	m->SetMesh(MeshAsset);
	m->SetMaterial(testMaterial);

	Transform* t = world->AddComponent<Transform>(coin);
	t->SetPosition(position);
	t->SetScale(VECTOR3_ONE);

	RotatorComponent* r = world->AddComponent<RotatorComponent>(coin);
	*r = RotatorComponent(Vector3(0, 100, 0));

	world->AddComponent<CoinPickup>(coin);

}

void CityBuilderLevel::CreatePlayerCamera()
{
	//Spawn Camera
	Entity cameraEntity = world->CreateEntity();

	CameraComponent* c = world->AddComponent<CameraComponent>(cameraEntity);

	c->fov = 45.f;
	c->farPlane = 1000.f;
	c->nearPlane = 0.1f;
	c->projectionType = ProjectionType::PERSPECTIVE;

	Transform* t = world->AddComponent<Transform>(cameraEntity);
	t->SetPosition(Vector3(0, 6, 23));
	t->SetRotation(Vector3(-10, -90, 0));

	CameraComponent::SetMainCamera(cameraEntity);

	FloatingCameraComponent* fcc = world->AddComponent<FloatingCameraComponent>(cameraEntity);
	*fcc = FloatingCameraComponent(5.f, 5.f);
}
