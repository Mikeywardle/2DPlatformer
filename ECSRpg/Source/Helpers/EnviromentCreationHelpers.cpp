#include "EnviromentCreationHelpers.h"

#include <Maths/Vector3.h>

#include <Core/SceneTransformComponents.h>
#include <Core/World.h>

#include <Components/GravityPanelComponent.h>

#include <Rendering/Meshes/MeshComponent.h>

#include <Physics/Collisions/CollisionComponents.h>

#include <Resources/ResourceManager.h>

#include <Config/NeOniPhysicsConfig.h>


namespace EnvironmentCreationHelpers
{
	Entity CreateGravityPanel(World* world, Vector3 Position, Vector3 rotation)
	{
		Entity PanelEntity = world->CreateEntity();

		SceneTransformComponent* transform = world->AddComponent<SceneTransformComponent>(PanelEntity);

		transform->SetPosition(Position);
		transform->SetRotation(rotation);

		ResourceManager* resourceManager = world->GetResourceManager();

		Mesh* meshAsset = resourceManager->GetMesh("GravityPanel");
		Material* material = resourceManager->GetMaterial("MarkerMaterial");

		MeshComponent* m = world->AddComponent<MeshComponent>(PanelEntity);
		m->SetMesh(meshAsset);
		m->SetMaterial(material);

		ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(PanelEntity);
		collider->isTrigger = true;

		AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
		geometry->HalfLimits = Vector3::One;

		collider->collisionLayer = NeOniCollisionLayers::Environment;

		collider->SetCollisionGeometry(geometry);

		world->AddComponent<StaticCollider>(PanelEntity);

		world->AddComponent<GravityPanelComponent>(PanelEntity);


		return PanelEntity;
	}

	Entity CreateFloatingPlatform(World* world, Vector3 Position, Vector3 rotation)
	{

	}
}
