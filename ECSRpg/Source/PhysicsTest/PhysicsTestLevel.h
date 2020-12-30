#pragma once

#include <Core/Level.h>
#include <Core/World.h>

#include <UI/Text.h>
#include <sstream>

#include <Debugging/FloatingCamera.h>
#include <Camera/CameraComponent.h>
#include <Rendering/Meshes/MeshComponent.h>#
#include <Physics/Collisions/AABB.h>
#include <Physics/Collisions/SphereCollider.h>
#include <Physics/RigidBody.h>
#include <Physics/Forces/Gravity.h>
#include <Resources/ResourceManager.h>

#include "PhysicsBallSpawnerSystem.h"

class PhysicsTestLevel : public Level
{
public:
	PhysicsTestLevel(World* world) : Level(world)
	{

	}

	virtual void OnStart() override
	{
		ResourceManager* resourceManager = world->GetResourceManager();

		Mesh* cubeMeshAsset = resourceManager->GetMesh("Cube");
		Mesh* sphereMeshAsset = resourceManager->GetMesh("Sphere");

		//Spawn Camera
		Entity cameraEntity = world->CreateEntity();

		CameraComponent* c = world->AddComponent<CameraComponent>(cameraEntity);

		c->fov = 45.f;
		c->farPlane = 1000.f;
		c->nearPlane = 0.1f;
		c->projectionType = ProjectionType::PERSPECTIVE;

		Transform* t = world->AddComponent<Transform>(cameraEntity);
		t->SetPosition(Vector3(0, 6, 50));
		t->SetRotation(Vector3(-10, -90, 0));

		CameraComponent::SetMainCamera(cameraEntity);

		FloatingCameraComponent* fcc = world->AddComponent<FloatingCameraComponent>(cameraEntity);
		*fcc = FloatingCameraComponent(5.f, 5.f);

		CreatePlatform(VECTOR3_ZERO, 20, 1, 20);

		CreatePlatform(Vector3(-20,10,0), 1, 10, 20);
		CreatePlatform(Vector3(20,10,0), 1, 10, 20);

		world->RemoveComponent<MeshComponent>(CreatePlatform(Vector3(0,10,20), 20, 10, 1));
		world->RemoveComponent<MeshComponent>(CreatePlatform(Vector3(0,10,-20), 20, 10, 1));


		world->RegisterSystem<FloatingCameraMovementSystem>();
		world->RegisterSystem<PhysicsBallSpawnerSystem>();
	}

	Entity CreatePlatform(Vector3 position, float width, float height, float depth)
	{
		ResourceManager* resourceManager = world->GetResourceManager();

		Mesh* cubeMeshAsset = resourceManager->GetMesh("Cube");
		Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

		//Spawn Mesh
		Entity mesh = world->CreateEntity();

		MeshComponent* m = world->AddComponent<MeshComponent>(mesh);
		m->SetMesh(cubeMeshAsset);
		m->SetMaterial(testMaterial);

		Transform* t4 = world->AddComponent<Transform>(mesh);
		t4->SetPosition(position);
		t4->SetScale(Vector3(width, height, depth));
		t4->SetStatic(true);

		AABBColliderComponent* sc = world->AddComponent<AABBColliderComponent>(mesh);
		new(sc) AABBColliderComponent(width, height, depth);

		return mesh;
	}

	virtual void OnFrame(float deltaTime) override
	{
		std::ostringstream ss;
		ss << 1 / deltaTime << "ms";
		RenderText(ss.str(), Vector2(20, 20), COLOR_WHITE);

		std::ostringstream ss2;
		ss2 <<world->GetEntities<SphereColliderComponent>().size() << " balls";
		RenderText(ss2.str(), Vector2(20, 40), COLOR_WHITE);
	}
};