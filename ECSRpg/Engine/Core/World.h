#pragma once

#include <vector>

#include <Utils/EngineDefines.h>
#include <Events/Event.h>

class GameContext;

class GameWindow;

class ECSContext;
class System;
class GameState;
class AudioEngine;
class RenderingSystem;
class ResourceManager;
class DebugSystem;
class EventsContext;
class Level;
class PhysicsSystem;
class SceneTransformSystem;

struct InputData;
struct PhysicsSystemConfig;

typedef unsigned int Entity;

#define ForEntities(World, ...) std::vector<Entity> entities = World->GetEntities<##__VA_ARGS__>(); for(Entity entity : entities)

class World
{
public:

	World(GameContext* InGame, PhysicsSystemConfig physicsConfig);
	~World();

#pragma region Levels
	//Levels
	template<typename T>
	T* SwitchLevel();
#pragma endregion

#pragma region ECS
	 //ECS functions
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void UnRegisterSystem(System* system);

	template<typename T>
	T* AddComponent(const Entity entity);

	template <typename T>
	void RemoveComponent(const Entity entity);

	template<typename T>
	T* GetComponent(const Entity entity) const;

	template<typename... Args>
	void FilterEntities(std::vector<Entity>& entities) const;

	template<typename... Args>
	std::vector<Entity> GetEntities() const;

	template<typename T>
	bool HasComponent(Entity entity) const;

	template<class T>
	T* RegisterSystem();

	template<typename T>
	std::vector<T>* GetComponents();

	template<typename ...Args>
	Entity GetEntity() const;

	template<typename T>
	Entity GetComponentOwner(const T* pointer) const;
#pragma endregion

#pragma region Events
	//Events Functions
	template<typename T, typename Clazz, typename Function>
	EventDelegate<T>* AddListener(Clazz* object, Function function);

	template<typename T>
	void RemoveListener(EventDelegate<T>* delegate);

	template<typename T>
	void DispatchMessage(T message);
#pragma endregion

	void RunFrame(const float deltaTime, const InputData* inputData);

	ResourceManager* GetResourceManager();
	AudioEngine* GetAudioEngine();

	GameState* GetGameState();

	void DeleteGameState();

	template<typename T>
	GameState* CreateNewGameState();

	GameContext* GetGame() const;
	GameWindow* GetGameWindow() const;
	PhysicsSystem* GetPhysicsSystem() const;


private:
	void BuildWorld();
	void ClearWorld();
	void DeRegisterAllSystems();

private:

	GameContext* game;

	RenderingSystem* renderingSystem;
	SceneTransformSystem* transformSystem;
	AudioEngine* audioEngine;
	PhysicsSystem* physicsSystem;

	GameState* gameState = nullptr;

	ECSContext* ecsContext;
	EventsContext* eventsContext;

	ResourceManager* resourceManager;

	Level* currentlyLoadedLevel = nullptr;
	std::vector<System*> systems;

//Debug stuff
#if NOT_RELEASE_BUILD

public:
	
	DebugSystem* GetDebugSystem();

private:
	void ProcessDebug(const float deltaTime, const InputData* inputData);

private:
	DebugSystem* debugSystem;

#endif

};

#include "World.inl"
