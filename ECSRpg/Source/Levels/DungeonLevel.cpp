#include "DungeonLevel.h"

#include <Core/World.h>
#include <Core/Transform.h>

#include <Camera/CameraComponent.h>

#include <Debugging/FloatingCamera.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Lighting/LightSources/DirectionalLight.h>

#include <Physics/RigidBody.h>
#include <Physics/Collisions/AABB.h>
#include <Physics/Collisions/SphereCollider.h>

#include <Camera/SpringArm.h>

#include <Systems/PlayerMovementSystem.h>
#include <Systems/CameraRotationSystem.h>

#include <Helpers/PlayerCreationHelpers.h>



void DungeonLevel::OnStart()
{

	SpawnPlayer(world);

	//CreatePlayerCamera();
	CreateDirectionalLight(Vector3(90, -30, 20));

	CreateTile(Vector3(5, 0, 5), Vector3(5, .2, 5), "TestFloor");

	CreateTile(Vector3(9, .7, 2), Vector3(.5, .5, 1), "TestMaterial");
	CreateTile(Vector3(8, .7, 6), Vector3(1, .5, 1), "TestMaterial");

	CreateTile(Vector3(5, 2, 0), Vector3(5, 2, .2), "TestWall");
	CreateTile(Vector3(5, 2, 10), Vector3(5, 2, .2), "TestWall");

	CreateTile(Vector3(0, 2, 5), Vector3(.2, 2, 5), "TestWall");
	CreateTile(Vector3(10, 2, 5), Vector3(.2, 2, 5), "TestWall");

	CreateMesh(Vector3(5, 5, 5), "SmoothSphere");
	CreateMesh(Vector3(8, 5, 8), "SmoothSphere");

	//world->RegisterSystem<FloatingCameraMovementSystem>();

	world->RegisterSystem<CameraRotationSystem>();
	world->RegisterSystem<SpringArmSystem>();
	world->RegisterSystem<PlayerMovementSystem>();
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
	t->SetPosition(Vector3(0, 10, -2));
	t->SetRotation(Vector3(0, 90, -45));

	CameraComponent::SetMainCamera(cameraEntity);

	FloatingCameraComponent* oc = world->AddComponent<FloatingCameraComponent>(cameraEntity);
	*oc = FloatingCameraComponent(10.f, .2f);

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
	t->SetScale(VECTOR3_ONE);
}
