#include "BulletCreationHelpers.h"

#include <Core/World.h>

#include <Resources/Mesh.h>
#include <Resources/Material.h>
#include <Resources/ResourceManager.h>

#include <Maths/Vector3.h>
#include <Maths/Transform.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Components/BulletTags.h>

#include <GamePlay/Lifetime.h>


void BulletCreation::SpawnPlayerBullet(World* world, Vector3 position, Vector3 heading)
{
	ResourceManager* resourceManager = world->GetResourceManager();

	Mesh* meshAsset = resourceManager->GetMesh("SmoothSphere");
	Material* testMaterial = resourceManager->GetMaterial("TestMaterial");

	Entity bullet = world->CreateEntity();

	Transform* transform = world->AddComponent<Transform>(bullet);
	transform->SetPosition(position);
	transform->SetScale(Vector3(0.2f, 0.2f, 0.2f));

	MeshComponent* mesh = world->AddComponent<MeshComponent>(bullet);
	mesh->SetMesh(meshAsset);
	mesh->SetMaterial(testMaterial);

	world->AddComponent<PlayerBullet>(bullet);

	BulletMovementComponent* bulletMovement = world->AddComponent<BulletMovementComponent>(bullet);
	bulletMovement->velocity = heading * 10.0f;

	LifetimeComponent* lifeTime = world->AddComponent<LifetimeComponent>(bullet);
	*lifeTime = LifetimeComponent(5.f, bullet);

}
