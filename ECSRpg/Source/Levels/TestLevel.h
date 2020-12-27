#pragma once
#include <Core/Level.h>

#include <ecs/EcsContext.h>
#include <Camera/CameraComponent.h>
#include <Core/Transform.h>
#include <Rendering/Sprites/Sprite.h>
#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/Sprites/BillBoardSprites.h>

#include <Physics/RigidBody.h>
#include <Physics/Forces/Gravity.h>
#include <Physics/Collisions/Collider.h>

#include <Debug/FloatingCamera.h>

#include <Resources/ResourceManager.h>

#include <GamePlay/Lifetime.h>

#include "../Components/RotatorComponent.h"

#include <stdio.h>

class TestLevel : public Level
{
public :
	TestLevel(World* world) : Level(world) {};

	Entity cameraEntity; 

	virtual void OnStart()
	{
		Level::OnStart();

		cameraEntity = world->CreateEntity();

		CameraComponent* c = world->AddComponent<CameraComponent>(cameraEntity);

		c->fov = 45.f;
		c->farPlane = 1000.f;
		c->nearPlane = 0.1f;
		c->projectionType = ProjectionType::PERSPECTIVE;


		Transform* t = world->AddComponent<Transform>(cameraEntity);
		t->SetPosition(Vector3(0, 0, -23));
		t->SetRotation(Vector3(0, 90, 0));

		CameraComponent::SetMainCamera(cameraEntity);

		FloatingCameraComponent* fcc = world->AddComponent<FloatingCameraComponent>(cameraEntity);
		*fcc = FloatingCameraComponent(2.f, 2.f);


		ResourceManager* resourceManager = world->GetResourceManager();

		int testTexture = resourceManager->LoadTexture("PlayerShip.png", true);
		int testShader = resourceManager->LoadShader("TestMesh", "BuildingVertex.vs", "BuildingFragment.fs");


		for (int i = 0; i <5; ++i)
		{
			Entity sprite = world->CreateEntity();

			Sprite* s = world->AddComponent<Sprite>(sprite);
			s->color = COLOR_GREEN;
			s->spriteTexture = testTexture;

			Transform* t2 = world->AddComponent<Transform>(sprite);
			t2->SetPosition(Vector3(i*2, 0, 0));
			t2->SetScale(VECTOR3_ONE);

			BillBoardComponent* b = world->AddComponent<BillBoardComponent>(sprite);

			b->rotateX = true;
			b->rotateY = true;
			b->rotateZ = true;

			LifetimeComponent* l = world->AddComponent<LifetimeComponent>(sprite);

			l->doesDecay = true;
			l->lifeRemaining = i * 2;

		}


		resourceManager->LoadMeshFromFile("test.fbx");

		Mesh* meshAsset = resourceManager->GetMesh("Cube");

		//----------------------------------------------------
		Entity mesh = world->CreateEntity();

		MeshComponent* m = world->AddComponent<MeshComponent>(mesh);
		m->SetMesh(meshAsset);
		m->SetMaterial(testShader);

		Transform* t4 = world->AddComponent<Transform>(mesh);
		t4->SetPosition(Vector3(0, 10, 0));
		t4->SetScale(VECTOR3_ONE);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(mesh);
		rb->mass = 10.f;

		GravityComponent* gc = world->AddComponent<GravityComponent>(mesh);
		gc->GravityScale = .5f;

		SphereColliderComponent* sc = world->AddComponent<SphereColliderComponent>(mesh);
		sc->radius = 1.f;


		Entity mesh2 = world->CreateEntity();

		m = world->AddComponent<MeshComponent>(mesh2);
		m->SetMesh(meshAsset);
		m->SetMaterial(testShader);

		t4 = world->AddComponent<Transform>(mesh2);
		t4->SetPosition(Vector3(0, 0, 0));
		t4->SetScale(VECTOR3_ONE);

		sc = world->AddComponent<SphereColliderComponent>(mesh2);
		sc->radius = 1.f;

	}

	virtual void OnFrame(float deltatime) override
	{
		Level::OnFrame(deltatime);
	}

};
