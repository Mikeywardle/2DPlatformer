#include "EnviromentCreationHelpers.h"

#include <Maths/Vector3.h>

#include <Core/SceneTransformComponents.h>
#include <Core/World.h>

#include <Components/GravityPanelComponent.h>
#include <Components/FloatingPlatformComponent.h>

#include <Rendering/Meshes/MeshComponent.h>
#include <Rendering/RenderDataComponents.h>

#include <Physics/Collisions/CollisionComponents.h>
#include <Physics/RigidBody.h>

#include <Resources/ResourceManager.h>

#include <Config/NeOniPhysicsConfig.h>

#include <Camera/CameraComponent.h>


namespace EnvironmentCreationHelpers
{
	Entity CreateFixedCamera(World* world, const Vector3& position)
	{
		Entity CameraEntity = world->CreateEntity();

		SceneTransformComponent* transform = world->AddComponent<SceneTransformComponent>(CameraEntity);

		transform->SetPosition(Vector3(0, 20, 0));
		transform->SetRotation(Vector3(0, 0, -90));

		CameraComponent* camera = world->AddComponent<CameraComponent>(CameraEntity);

		camera->fov = 45.f;
		camera->farPlane = 1000.f;
		camera->nearPlane = 0.1f;
		camera->projectionType = ProjectionType::PERSPECTIVE;

		CameraComponent::SetMainCamera(CameraEntity);

		return CameraEntity;
	}

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

	Entity CreateFloatingPlatform(World* world
		, Vector3 position
		, Vector3 rotation
		, Vector3 scale
		, Vector3 desitnation)
	{
		Entity PlatformEntity = world->CreateEntity();

		SceneTransformComponent* transform = world->AddComponent<SceneTransformComponent>(PlatformEntity);

		transform->SetPosition(position);
		transform->SetRotation(rotation);

		ResourceManager* resourceManager = world->GetResourceManager();

		Mesh* meshAsset = resourceManager->GetMesh("Cube");
		Material* material = resourceManager->GetMaterial("TestFloor");

		MeshComponent* m = world->AddComponent<MeshComponent>(PlatformEntity);
		m->SetMesh(meshAsset);
		m->SetMaterial(material);

		ColliderGeometryComponent* collider = world->AddComponent<ColliderGeometryComponent>(PlatformEntity);

		AABBCollisionGeometry* geometry = new AABBCollisionGeometry();
		geometry->HalfLimits = scale;

		collider->collisionLayer = NeOniCollisionLayers::Environment;

		collider->SetCollisionGeometry(geometry);

		world->AddComponent<DynamicCollider>(PlatformEntity);

		RenderScale* renderScale = world->AddComponent<RenderScale>(PlatformEntity);
		renderScale->scale = scale;

		RigidBodyComponent* rb = world->AddComponent<RigidBodyComponent>(PlatformEntity);
		rb->isInfiniteMass = true;
		rb->Friction = 3.f;

		FloatingPlatformComponent* fpc = world->AddComponent<FloatingPlatformComponent>(PlatformEntity);
		fpc->a = position;
		fpc->b = desitnation;
		fpc->moveSpeed = 2.0f;

		return PlatformEntity;
	}
}
