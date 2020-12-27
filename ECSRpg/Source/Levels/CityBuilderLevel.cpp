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

#include <Debug/FloatingCamera.h>

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

struct TestBallTag {};

void CityBuilderLevel::OnStart()
{
	Level::OnStart();

	ResourceManager* resourceManager = world->GetResourceManager();

	int testShader = resourceManager->LoadShader("TestMesh", "BuildingVertex.vs", "BuildingFragment.fs");

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
	Entity sphere = world->GetEntities<TestBallTag>()[0];

	Transform* t = world->GetComponent<Transform>(sphere);
	PlayerMovementComponent* p = world->GetComponent<PlayerMovementComponent>(sphere);

	std::ostringstream ss2;
	ss2 << 1/deltaTime << "fps";
	RenderText(ss2.str(), Vector2(20, 40), COLOR_WHITE);
	 
	std::ostringstream ss3;
	ss3 << "x: "<<t->GetPosition().x << ", y: "<< t->GetPosition().y;
	RenderText(ss3.str(), Vector2(20, 60), COLOR_WHITE);

	std::ostringstream ss4;
	ss4 << p->CurrentJumpsNumber <<" jumps";
	RenderText(ss4.str(), Vector2(20, 20), COLOR_WHITE);

	if (p->inAir)
	{
		RenderText("In Air", Vector2(20, 80), COLOR_WHITE);
	}


}

void CityBuilderLevel::CreatePlayer()
{	
	ResourceManager* resourceManager = world->GetResourceManager();
	//Mesh* sphereMeshAsset = resourceManager->GetMesh("Sphere");
	Texture2D* spriteTexture = resourceManager->GetTexture("PlayerShip");

	int testShader = resourceManager->GetShader("TestMesh");

	Entity player = world->CreateEntity();

	//MeshComponent *m = world->AddComponent<MeshComponent>(player);
	//m->SetMesh(sphereMeshAsset);
	//m->SetMaterial(testShader);

	SpriteComponent* renderer = world->AddComponent<SpriteComponent>(player);
	*renderer = SpriteComponent(spriteTexture, COLOR_WHITE, 32);

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
	rb->Restitution = .07f;
	rb->Friction = 0.5f;

	GravityComponent* g = world->AddComponent<GravityComponent>(player);
	g->GravityScale = 1.f;

	AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
	a->DragCoefficient = 1.f;

	PlayerMovementComponent* p = world->AddComponent<PlayerMovementComponent>(player);
	new(p) PlayerMovementComponent(2,100,80,.5);

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
	int testShader = resourceManager->GetShader("TestMesh");

	//Spawn Mesh
	Entity mesh = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(mesh);
	m->SetMesh(cubeMeshAsset);
	m->SetMaterial(testShader);

	Transform* t4 = world->AddComponent<Transform>(mesh);
	t4->SetPosition(position);
	t4->SetScale(Vector3(width, height, 5));
	t4->SetStatic(true);

	AABBColliderComponent* sc = world->AddComponent<AABBColliderComponent>(mesh);
	new(sc) AABBColliderComponent(width, height, 5);

	return mesh;
}

void CityBuilderLevel::CreateFloatingPlatform(Vector3 position, float width, float height, Vector3 start, Vector3 end)
{
	Entity platform = CreatePlatform(position, width, height);
	world->GetComponent<Transform>(platform)->SetStatic(false);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(platform);
	rb->isInfiniteMass = true;

	FloatingPlatformComponent* f = world->AddComponent<FloatingPlatformComponent>(platform);
	*f = FloatingPlatformComponent(start, end, 2.f);
}

void CityBuilderLevel::CreateCoin(Vector3 position)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* MeshAsset = resourceManager->GetMesh("Cylinder");
	int testShader = resourceManager->GetShader("TestMesh");

	//Spawn Mesh
	Entity coin = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(coin);
	m->SetMesh(MeshAsset);
	m->SetMaterial(testShader);

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
