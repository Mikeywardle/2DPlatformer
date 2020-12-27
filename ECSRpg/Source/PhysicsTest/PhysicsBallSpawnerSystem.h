#pragma once

#include <ecs/System.h>
#include <Core/World.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Physics/Collisions/SphereCollider.h>
#include <Physics/RigidBody.h>
#include <Physics/Forces/Gravity.h>
#include <Physics/Forces/AirResistance.h>
#include <Resources/ResourceManager.h>


class PhysicsBallSpawnerSystem : public System
{
public:
	PhysicsBallSpawnerSystem(World* world) : System(world)
	{
		timeRemaining = SpawnTime;
	}

	virtual void OnFrame(float deltaTime) override
	{
		if (SpawningBalls)
		{
			timeRemaining -= deltaTime;
			if (timeRemaining <= 0)
			{
				timeRemaining = SpawnTime;
				CreateBall();
			}
		}

	}

	void CreateBall()
	{
		float xPos = (float(rand()) / float(RAND_MAX) * 20)-10;
		float yPos = (float(rand()) / float(RAND_MAX) * 20)-10;

		ResourceManager* resourceManager = world->GetResourceManager();
		Mesh* sphereMeshAsset = resourceManager->GetMesh("Sphere");

		int testShader = resourceManager->GetShader("TestMesh");

		Entity player = world->CreateEntity();

		MeshComponent* m = world->AddComponent<MeshComponent>(player);
		m->SetMesh(sphereMeshAsset);
		m->SetMaterial(testShader);

		Transform* t4 = world->AddComponent<Transform>(player);
		t4->SetPosition(Vector3(xPos, 10, yPos));
		t4->SetScale(VECTOR3_ONE);

		SphereColliderComponent* sphere = world->AddComponent<SphereColliderComponent>(player);
		new(sphere) SphereColliderComponent(1);

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(player);
		rb->mass = 10;
		rb->Restitution = .8f;

		GravityComponent* g = world->AddComponent<GravityComponent>(player);
		g->GravityScale = 1.5f;

		//AirResistanceComponent* a = world->AddComponent<AirResistanceComponent>(player);
		//a->DragCoefficient = 1;
	}

	bool SpawningBalls = true;
	float SpawnTime = .1f;
	float timeRemaining;

};
