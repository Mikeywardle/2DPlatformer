#include "World.h"
#include <Rendering/RenderingSystem.h>
#include <Physics/PhysicsSystem.h>
#include <ecs/EcsContext.h>
#include <ecs/System.h>
#include <Resources/ResourceManager.h>
#include <Audio/AudioEngine.h>
#include <Utils/STLUtils.h>
#include <Debugging/DebugSystem.h>

World::World()
{
	ecsContext = new ECSContext();
	eventsContext = new EventsContext();

	renderingSystem = new RenderingSystem(this);
	physicsSystem = new PhysicsSystem(this);
	debugSystem = new DebugSystem(this);

	resourceManager = new ResourceManager(this);
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
	ProcessDebug(deltaTime);

}

void World::ProcessPhysics(float deltaTime)
{
	physicsSystem->ProcessPhysicsForFrame(deltaTime);
}

void World::DrawWorld()
{
	renderingSystem->DrawWorld();
}

void World::ProcessDebug(float deltaTime)
{
	debugSystem->OnFrame(deltaTime);
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
