#include "World.h"

#include <Game.h>

#include <Rendering/RenderingSystem.h>
#include <ecs/EcsContext.h>
#include <ecs/System.h>
#include <Resources/ResourceManager.h>
#include <Audio/AudioEngine.h>
#include <Utils/STLUtils.h>

#include <Physics/Collisions/CollisionSystem.h>
#include <Core/GameState.h>
#include <Physics/PhysicsSystem.h>

#if NOT_RELEASE_BUILD
#include <Debugging/DebugSystem.h>
#endif

World::World(GameContext* InGame)
{
	game = InGame;

	ecsContext = new ECSContext();
	eventsContext = new EventsContext();

	physicsSystem = new PhysicsSystem(this);
	renderingSystem = new RenderingSystem(this);

	resourceManager = new ResourceManager(this);

#if NOT_RELEASE_BUILD
	debugSystem = new DebugSystem(this);
#endif
}

World::~World()
{
	delete(ecsContext);
	delete(eventsContext);
	delete(physicsSystem);
	delete(renderingSystem);
	delete(resourceManager);

#if NOT_RELEASE_BUILD
	delete(debugSystem);
#endif
}

void World::UnRegisterSystem(System* system)
{
	RemoveElementFromVector(systems, system);
	delete(system);
}

void World::RunFrame(const float deltaTime, const InputData* inputData)
{
	//Update Physics
	physicsSystem->ProcessPhysicsForFrame(deltaTime);

	//Iterate through systems
	for (System* system : systems)
	{
		system->OnInput(deltaTime, inputData);
	}
	for (System* system : systems)
	{
		system->OnFrame(deltaTime);
	}

	//Process Level
	currentlyLoadedLevel->OnFrame(deltaTime);

	//Draw World
	renderingSystem->DrawWorld();

#if NOT_RELEASE_BUILD
	ProcessDebug(deltaTime, inputData);
#endif
}

GameContext* World::GetGame() const
{
	return game;
}

GameWindow* World::GetGameWindow() const
{
	return game->GetGameWindow();
}

PhysicsSystem* World::GetPhysicsSystem() const
{
	return physicsSystem;
}


void World::BuildWorld()
{
	physicsSystem->GenerateStaticWorld();
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

AudioEngine* World::GetAudioEngine()
{
	return audioEngine;
}

GameState* World::GetGameState()
{
	return gameState;
}

void World::DeleteGameState()
{
	if (gameState != nullptr)
		delete(gameState);
}

#if NOT_RELEASE_BUILD
DebugSystem* World::GetDebugSystem()
{
	return debugSystem;
}
void World::ProcessDebug(const float deltaTime, const InputData* inputData)
{
	debugSystem->OnFrame(deltaTime, inputData);
}
#endif