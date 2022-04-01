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
#include <Physics/Forces/Gravity.h>

#include <Systems/BattleCameraSystem.h>
#include <Systems/MouseSelectionSystem.h>

#include <Systems/DungeonPlayerMovementSystem.h>

#include <GamePlay/TransformParenting.h>
#include <GamePlay/Lifetime.h>

#include <Data/NeOniCollisionLayers.h>

#include <Components/BattleCameraComponent.h>
#include <Components/UnitComponents.h>

#include <Helpers/PlayerCreationHelpers.h>



void DungeonLevel::LoadLevel()
{
	CreateDirectionalLight(Vector3(80, -10, 30));

	//Floor
	CreateTile(Vector3(5, -.2f, 5), Vector3(10, .2, 10), "TestFloor", true);

	//Blocks
	CreateTile(Vector3(9, .5, 2), Vector3(.5, .5, 1), "TestMaterial", false);
	CreateTile(Vector3(8, .5, 6), Vector3(1, .5, 1), "TestMaterial", false);

	//Wall
	CreateTile(Vector3(5, 2, -5), Vector3(10, 2, .2), "TestWall", false);
	CreateTile(Vector3(5, 2, 15), Vector3(10, 2, .2), "TestWall", false);

	CreateTile(Vector3(-5, 2, 5), Vector3(.2, 2, 10), "TestWall", false);
	CreateTile(Vector3(15, 2, 5), Vector3(.2, 2, 10), "TestWall", false);

	//Mouse marker
	CreateMouseSelectionMarker();

	for (int i = -2; i < 8; ++i)
	{
		for (int j = -2; j < 8; ++j)
		{
			CreateFloorSectionMarker(Vector3(i * 2, 0, j * 2));
		}
	}

	//Units
	CreateUnit(Vector3(0, 5, 0));

	//Spawn Player
	//PlayerCreation::SpawnDungeonPlayer(world, Vector3(0,0,0));

	CreatePlayerCamera();
}

void DungeonLevel::OnStart()
{
	world->RegisterSystem<BattleCameraSystem>();
	world->RegisterSystem<MouseSelectionSystem>();

	//PlayerSystems
	world->RegisterSystem<DungeonPlayerMovementSystem>();

	//Gameplay Utils
	world->RegisterSystem<PositionAttachmentSystem>();
	world->RegisterSystem<LifeTimeDecaySystem>();


}

void DungeonLevel::OnInput(float deltaTime, const InputData* inputData)
{
	//if (inputData->GetInputValue(TestConfigInputId::Right_click, InputTypes::BUTTON_RELEASED))
	//{
	//	printf("Reload level");
	//	world->SwitchLevel<DungeonLevel>();
	//}

}

void DungeonLevel::CreatePlayerCamera()
{
	Entity entity = world->CreateEntity();

	Transform* transform = world->AddComponent<Transform>(entity);

	transform->SetPosition(Vector3(0, 20, 0));
	transform->SetRotation(Vector3(0, 0, -90));

	CameraComponent* camera = world->AddComponent<CameraComponent>(entity);

	camera->fov = 45.f;
	camera->farPlane = 1000.f;
	camera->nearPlane = 0.1f;
	camera->projectionType = ProjectionType::PERSPECTIVE;

	CameraComponent::SetMainCamera(entity);

	BattleCameraComponent* battleCamera = world->AddComponent<BattleCameraComponent>(entity);

	battleCamera->MovementSpeed = 10.f;
	battleCamera->RotationSpeed = 10.f;
}

void DungeonLevel::CreateUnit(Vector3 position)
{
	Entity unit = CreateMesh(position, "SmoothSphere", "UnitTestMaterial");

	world->AddComponent<SelectableUnitComponent>(unit);

	SphereCollisionGeometry* geometry = new SphereCollisionGeometry();
	geometry->radius = 1.0f;

	ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(unit);

	collider->collisionLayer = NeOniCollisionLayers::Players;
	collider->toCollideLayers.push_back(NeOniCollisionLayers::Environment);
	collider->toCollideLayers.push_back(NeOniCollisionLayers::Floors);

	collider->SetCollisionGeometry(geometry);

	world->AddComponent<DynamicCollider>(unit);

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(unit);
	rb->mass = 10;
	rb->Restitution = .01f;
	rb->Friction = 1.f;

	GravityComponent* gravity = world->AddComponent<GravityComponent>(unit);
}

void DungeonLevel::CreateMouseSelectionMarker()
{
	Entity marker = CreateMesh(Vector3(0,2,0), "Sphere", "MarkerMaterial");

	Transform* transform = world->GetComponent<Transform>(marker);
	transform->SetScale(Vector3(0.1f, 0.1f, 0.1f));

	world->AddComponent<MouseSelectionDebugMarker>(marker);
}

void DungeonLevel::CreateFloorSectionMarker(Vector3 position)
{
	Entity marker = CreateMesh(position, "Sphere", "MarkerMaterial");

	Transform* transform = world->GetComponent<Transform>(marker);
	transform->SetScale(Vector3(0.15f, 0.15f, 0.15f));
}

void DungeonLevel::CreateTile(Vector3 Position, Vector3 Scale, std::string materialName, bool isFloor)
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

	AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
	geometry->HalfLimits = Vector3::One;

	ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(platform);

	if (isFloor)
	{
		collider->collisionLayer = NeOniCollisionLayers::Floors;
	}
	else
	{
		collider->collisionLayer = NeOniCollisionLayers::Environment;
	}

	collider->SetCollisionGeometry(geometry);
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

Entity DungeonLevel::CreateMesh(Vector3 position, std::string mesh, std::string material)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* meshAsset = resourceManager->GetMesh(mesh);
	Material* testMaterial = resourceManager->GetMaterial(material);

	//Spawn Mesh
	Entity platform = world->CreateEntity();

	MeshComponent* m = world->AddComponent<MeshComponent>(platform);
	m->SetMesh(meshAsset);
	m->SetMaterial(testMaterial);

	Transform* t = world->AddComponent<Transform>(platform);
	t->SetPosition(position);
	t->SetStatic(false);
	t->SetScale(Vector3::One);

	return platform;
}
