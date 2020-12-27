#include "World.h"
#include <Rendering/RenderingSystem.h>
#include <Physics/PhysicsSystem.h>
#include <ecs/EcsContext.h>
#include <ecs/System.h>
#include <Resources/ResourceManager.h>
#include <Audio/AudioEngine.h>
#include <Utils/STLUtils.h>

World::World()
{
	ecsContext = new ECSContext();
	eventsContext = new EventsContext();

	renderingSystem = new RenderingSystem(this);
	physicsSystem = new PhysicsSystem(this);

	resourceManager = new ResourceManager();
}

void World::UnRegisterSystem(System* system)
{
	RemoveElementFromVector(systems, system);
	delete(system);
}

void World::RunFrame(float deltaTime)
{
	for (System* system : systems)
	{
		system->OnFrame(deltaTime);
	}

	for (Level* level : currentlyLoadedLevels)
	{
		level->OnFrame(deltaTime);
	}

	ProcessPhysics(deltaTime);
	DrawWorld();
}

void World::ProcessPhysics(float deltaTime)
{
	physicsSystem->ProcessPhysicsForFrame(deltaTime);
}

void World::DrawWorld()
{
	renderingSystem->DrawWorld();
}

Entity World::CreateEntity()
{
	return ecsContext->CreateEntity();
}

void World::DestroyEntity(Entity entity)
{
	ecsContext->DestroyEntity(entity);
}

ResourceManager* World::GetResourceManager()
{
	return resourceManager;
}

PhysicsSystem* World::GetPhysicsSystem()
{
	return physicsSystem;
}

AudioEngine* World::GetAudioEngine()
{
	return audioEngine;
}
