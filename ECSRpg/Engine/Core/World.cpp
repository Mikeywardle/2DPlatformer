#include "World.h"

#include <Game.h>

#include <Core/SceneTransformSystem.h>

#include <Rendering/RenderingSystem.h>
#include <ecs/EcsContext.h>
#include <ecs/System.h>
#include <Resources/ResourceManager.h>
#include <Audio/AudioEngine.h>
#include <Utils/STLUtils.h>

#include <Physics/Collisions/CollisionSystem.h>
#include <Core/GameState.h>
#include <Physics/PhysicsSystem.h>
#include <Physics/PhysicsSystemConfig.h>

#include <Analytics/Timer.h>

#if NOT_RELEASE_BUILD
#include <Debugging/DebugSystem.h>
#endif

World::World(GameContext* InGame, PhysicsSystemConfig physicsConfig)
{
	game = InGame;

	ecsContext = new ECSContext();
	eventsContext = new EventsContext();

	physicsSystem = new PhysicsSystem(this, physicsConfig);
	renderingSystem = new RenderingSystem(this);

	resourceManager = new ResourceManager(this);

	transformSystem = new SceneTransformSystem(this);

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
	delete (transformSystem);

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

	Timer timer = Timer();

	timer.StartTimer();
	//Update Physics
	physicsSystem->ProcessPhysicsForFrame(deltaTime);

	const double physTime = timer.StopTimer();

	timer.StartTimer();
	//Iterate through systems
	for (System* system : systems)
	{
		system->OnInput(deltaTime, inputData);
	}
	for (System* system : systems)
	{
		system->OnFrame(deltaTime);
	}
	const double sysTime = timer.StopTimer();

	//Process Level
	currentlyLoadedLevel->OnInput(deltaTime, inputData);
	currentlyLoadedLevel->OnFrame(deltaTime);

	transformSystem->Update(deltaTime);
	timer.StartTimer();
	//Draw World
	renderingSystem->DrawWorld();

	const double drawTime = timer.StopTimer();

	printf("Phys- %.3fms, Sys- %.3fms, Draw- %.3fms\n", physTime, sysTime, drawTime);

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

void World::ClearWorld()
{
	ecsContext->ClearEntities();
	physicsSystem->ClearWorld();
	resourceManager->ClearResources();
}

void World::DeRegisterAllSystems()
{
	for (System* system : systems)
	{
		delete(system);
	}

	systems.clear();
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