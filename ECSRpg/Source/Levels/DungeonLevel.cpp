#include "DungeonLevel.h"

#include <Core/World.h>
#include <Core/SceneTransformComponents.h>

#include <Camera/CameraComponent.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/RenderDataComponents.h>

#include <Lighting/LightSources/DirectionalLight.h>

#include <Physics/RigidBody.h>
#include <Physics/Collisions/CollisionComponents.h>
#include <Physics/Forces/Gravity.h>

#include <Systems/BattleCameraSystem.h>
#include <Systems/MouseSelectionSystem.h>

#include <Systems/PlayerMovementSystem.h>
#include <Systems/GravityPanelSystem.h>
#include <Systems/FloatingPlatformSystem.h>

#include <GamePlay/TransformParenting.h>
#include <GamePlay/Lifetime.h>

#include <Data/NeOniCollisionLayers.h>

#include <Components/BattleCameraComponent.h>
#include <Components/UnitComponents.h>

#include <Helpers/PlayerCreationHelpers.h>
#include <Helpers/EnviromentCreationHelpers.h>

#include <Inputs/InputData.h>
#include <Inputs/InputValues.h>

#include <Config/TestInputConfig.h>



void DungeonLevel::LoadLevel()
{
	CreateDirectionalLight(Vector3(80, -10, 30));

	//Floor
	CreateTile(Vector3(5, -.2f, 5), Vector3(10, .2, 10), "TestFloor", true);
	CreateTile(Vector3(7.5, -.2f, 85), Vector3(15, .2, 10), "TestFloor", true);

	//Ceiling
	CreateTile(Vector3(-3, 16.0f, 10), Vector3(2, .2, 20), "TestFloor", true);
	CreateTile(Vector3(14, 16.0f, 60), Vector3(2, .2, 30), "TestFloor", true);
	//CreateTile(Vector3(5, 2.2f, 5), Vector3(10, .2, 10), "TestFloor", true);

	//Blocks
	CreateTile(Vector3(9, .5, 2), Vector3(.5, .5, 1), "TestMaterial", false);
	CreateTile(Vector3(8, .5, 6), Vector3(1, .5, 1), "TestMaterial", false);

	//Wall
	CreateTile(Vector3(5, 2, -5), Vector3(10, 2, .2), "TestWall", false);
	//CreateTile(Vector3(5, 2, 15), Vector3(10, 2, .2), "TestWall", false);

	CreateTile(Vector3(-5, 2, 5), Vector3(.2, 2, 10), "TestWall", false);
	CreateTile(Vector3(15, 2, 5), Vector3(.2, 2, 10), "TestWall", false);

	EnvironmentCreationHelpers::CreateGravityPanel(world, Vector3(-3, 0, 10), Vector3::Zero);
	EnvironmentCreationHelpers::CreateGravityPanel(world, Vector3(14, 15.8f, 80), Vector3(180,0,0));

	EnvironmentCreationHelpers::CreateFloatingPlatform(world, Vector3(-3, 16.0f, 32), Vector3::Zero, Vector3(2, .2f, 2), Vector3(10, 16.0f, 32));

	//Part 2
	EnvironmentCreationHelpers::CreateFloatingPlatform(world, Vector3(35, .5, 85), Vector3::Zero, Vector3(2, .2f, 2), Vector3(35, 4, 85));

	CreateTile(Vector3(150, -.2f, 85), Vector3(8, .2, 20), "TestFloor", true);

	//Mouse marker
	CreateMouseSelectionMarker();

	//Spawn Player
	PlayerCreation::SpawnPlayer(world, Vector3(0,0,0));

	//CreatePlayerCamera();

	CreateAtom(Vector3(15, 15, 15));
}

void DungeonLevel::OnStart()
{
	world->RegisterSystem<BattleCameraSystem>();
	world->RegisterSystem<MouseSelectionSystem>();

	//PlayerSystems
	world->RegisterSystem<PlayerMovementSystem>();
	world->RegisterSystem<GravityPanelSystem>();
	world->RegisterSystem<FloatingPlatformSystem>();

	//Gameplay Utils
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

void DungeonLevel::OnFrame(float deltaTime)
{
}

void DungeonLevel::CreatePlayerCamera()
{
	Entity entity = world->CreateEntity();

	SceneTransformComponent* transform = world->AddComponent<SceneTransformComponent>(entity);

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
	//Create Marker
	Entity marker = CreateMesh(Vector3(0,2,0), "Sphere", "MarkerMaterial");

	RenderScale* markerRenderScale = world->AddComponent<RenderScale>(marker);
	markerRenderScale->scale = Vector3(0.1f, 0.1f, 0.1f);

	world->AddComponent<MouseSelectionDebugMarker>(marker);

}

void DungeonLevel::CreateFloorSectionMarker(Vector3 position)
{
	Entity marker = CreateMesh(position, "Sphere", "MarkerMaterial");

	SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(marker);

	RenderScale* renderScale = world->AddComponent<RenderScale>(marker);
	renderScale->scale = Vector3(0.15f, 0.15f, 0.15f);
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

	SceneTransformComponent* t4 = world->AddComponent<SceneTransformComponent>(platform);
	t4->SetPosition(Position);
	t4->SetStatic(true);

	RenderScale* renderScale = world->AddComponent<RenderScale>(platform);
	renderScale->scale = Scale;

	RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(platform);
	rb->isInfiniteMass = true;
	rb->Friction = 3.f;

	AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
	geometry->HalfLimits = Scale;

	ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(platform);

	//if (isFloor)
	//{
	//	collider->collisionLayer = NeOniCollisionLayers::Floors;
	//}
	//else
	//{
	//	collider->collisionLayer = NeOniCollisionLayers::Environment;
	//}

	collider->collisionLayer = NeOniCollisionLayers::Environment;

	collider->SetCollisionGeometry(geometry);
	world->AddComponent<StaticCollider>(platform);
}

void DungeonLevel::CreateDirectionalLight(Vector3 rotation)
{
	Entity light = world->CreateEntity();

	DirectionalLightComponent* dl = world->AddComponent<DirectionalLightComponent>(light);
	*dl = DirectionalLightComponent(COLOR_WHITE, .05);

	SceneTransformComponent* t = world->AddComponent<SceneTransformComponent>(light);
	t->SetRotation(rotation);
}

void DungeonLevel::CreateAtom(Vector3 position)
{
	//Create Marker
	Entity AtomEntity = CreateMesh(position, "SmoothSphere", "UnitTestMaterial");

	RenderScale* markerRenderScale = world->AddComponent<RenderScale>(AtomEntity);
	markerRenderScale->scale = Vector3(.8f, .8f, .8f);

	world->AddComponent<MouseSelectionDebugMarker>(AtomEntity);

	//Create rotator
	Entity rotatorEntity = world->CreateEntity();

	SceneTransformComponent* rotatorTransform = world->AddComponent<SceneTransformComponent>(rotatorEntity);
	rotatorTransform->SetRotation(Vector3(0, 0, 45));

	//Create rotator 2
	Entity rotator2Entity = world->CreateEntity();

	SceneTransformComponent* rotator2Transform = world->AddComponent<SceneTransformComponent>(rotator2Entity);
	rotator2Transform->SetRotation(Vector3(0, 0, 315));

	//Create Accesory
	Entity markerAccesory = CreateMesh(Vector3(0, 2, 0), "SmoothSphere", "TestMaterial");

	SceneTransformComponent* accessoryTransform = world->GetComponent<SceneTransformComponent>(markerAccesory);

	RenderScale* accessoryRenderScale = world->AddComponent<RenderScale>(markerAccesory);
	accessoryRenderScale->scale = Vector3(0.2f, 0.2f, 0.2f);

	//Create Accesory 2
	Entity marker2Accesory = CreateMesh(Vector3(0, 2.f, 0), "SmoothSphere", "TestMaterial");

	SceneTransformComponent* accessory2Transform = world->GetComponent<SceneTransformComponent>(marker2Accesory);


	RenderScale* accessoryRenderScale2 = world->AddComponent<RenderScale>(marker2Accesory);
	accessoryRenderScale2->scale = Vector3(0.2f, 0.2f, 0.2f);


	//Re-get transforms
	SceneTransformComponent* atomTransform = world->GetComponent<SceneTransformComponent>(AtomEntity);
	rotatorTransform = world->GetComponent<SceneTransformComponent>(rotatorEntity);
	rotator2Transform = world->GetComponent<SceneTransformComponent>(rotator2Entity);
	accessoryTransform = world->GetComponent<SceneTransformComponent>(markerAccesory);
	accessory2Transform = world->GetComponent<SceneTransformComponent>(marker2Accesory);

	//Setup transform hierachy
	atomTransform->AttachChild(rotatorTransform);
	atomTransform->AttachChild(rotator2Transform);
	rotatorTransform->AttachChild(accessoryTransform);
	rotator2Transform->AttachChild(accessory2Transform);
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

	SceneTransformComponent* t = world->AddComponent<SceneTransformComponent>(platform);

	t->SetPosition(position);
	t->SetStatic(false);


	return platform;
}
