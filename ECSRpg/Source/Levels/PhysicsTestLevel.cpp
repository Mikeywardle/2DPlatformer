#include "PhysicsTestLevel.h"


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

#include <GamePlay/TransformParenting.h>
#include <GamePlay/Lifetime.h>

#include <Data/NeOniCollisionLayers.h>

#include <Components/BattleCameraComponent.h>

void PhysicsTestlevel::LoadLevel()
{
	CreatePlayerCamera();

	CreateDirectionalLight(Vector3(40, -10, 30));

	const int worldSize = 50;
	const float worldSpacing = 3.0f;
	//Floor
	for (int z = 0; z < worldSize; ++z)
	{
		for (int y = 0; y < worldSize; ++y)
		{
			for (int x = 0; x < worldSize; ++x)
			{
				CreateTile(Vector3(x * worldSpacing, y * worldSpacing, z * worldSpacing), Vector3(.5f, .5f, .5f), "TestMaterial");
			}
		}
	}


	//Mouse marker
	CreateMouseSelectionMarker();
}

void PhysicsTestlevel::OnStart()
{
	world->RegisterSystem<BattleCameraSystem>();
	world->RegisterSystem<MouseSelectionSystem>();

	//Gameplay Utils
	world->RegisterSystem<PositionAttachmentSystem>();
	world->RegisterSystem<LifeTimeDecaySystem>();
}

void PhysicsTestlevel::CreatePlayerCamera()
{
	Entity entity = world->CreateEntity();

	Transform* transform = world->AddComponent<Transform>(entity);

	transform->SetPosition(Vector3(0, 5, -6));
	transform->SetRotation(Vector3(0, 0, -30));

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

void PhysicsTestlevel::CreateMouseSelectionMarker()
{
	Entity marker = CreateMesh(Vector3(0, 2, 0), "Sphere", "MarkerMaterial");

	Transform* transform = world->GetComponent<Transform>(marker);
	transform->SetScale(Vector3(0.1f, 0.1f, 0.1f));

	world->AddComponent<MouseSelectionDebugMarker>(marker);
}

Entity PhysicsTestlevel::CreateMesh(Vector3 position, std::string mesh, std::string material)
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

void PhysicsTestlevel::CreateDirectionalLight(Vector3 rotation)
{
	Entity light = world->CreateEntity();

	DirectionalLightComponent* dl = world->AddComponent<DirectionalLightComponent>(light);
	*dl = DirectionalLightComponent(COLOR_WHITE, .05);

	Transform* t = world->AddComponent<Transform>(light);
	t->SetRotation(rotation);
}

void PhysicsTestlevel::CreateTile(Vector3 Position, Vector3 Scale, std::string materialName)
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

	//RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(platform);
	//rb->isInfiniteMass = true;
	//rb->Friction = 3.f;

	AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
	geometry->HalfLimits = Scale;

	ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(platform);

	collider->collisionLayer = NeOniCollisionLayers::Environment;

	collider->SetCollisionGeometry(geometry);

	world->AddComponent<StaticCollider>(platform);
}
