#pragma once

#include <vector>
#include "Level.h"
#include <ecs/System.h>
#include <ecs/EcsContext.h>

#include <ecs/Entity.h>
#include <Events/EventsContext.h>

class World
{
public:

	World();

#pragma region Levels
	//Levels
	template<typename T>
	T* SwitchLevel()
	{
		T* newLevel = new T(this);
		Level* toAdd = dynamic_cast<Level*>(newLevel);
		currentlyLoadedLevels.push_back(newLevel);

		toAdd->OnStart();
		return newLevel;

	}
#pragma endregion

#pragma region ECS
	 //ECS functions
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	template<typename T>
	T* AddComponent(const Entity entity)
	{
		return ecsContext->AddComponent<T>(entity);
	}

	template <typename T>
	void RemoveComponent(const Entity entity)
	{
		ecsContext->RemoveComponent<T>(entity);
	}

	template<typename T>
	T* GetComponent(const Entity entity) const
	{
		return ecsContext->GetComponent<T>(entity);
	}

	template<typename... Args>
	void FilterEntities(std::vector<Entity>& entities) const
	{
		ecsContext->FilterEntities<Args...>(entities);
	}

	template<typename... Args>
	std::vector<Entity> GetEntities() 
	{
		return ecsContext->GetEntities<Args...>();
	}
	template<typename T>
	bool HasComponent(Entity entity) const
	{
		return ecsContext->HasComponent<T>(entity);
	}

	template<class T>
	T*  RegisterSystem()
	{
		T* toReturn = new T(this);
		systems.push_back(toReturn);

		return toReturn;
	}

	void UnRegisterSystem(System* system);

	template<typename T>
	std::vector<T>* GetComponents()
	{
		return ecsContext->GetComponents<T>();
	}
#pragma endregion

#pragma region Events
	//Events Functions
	template<typename T, typename Clazz, typename Function>
	EventDelegate<T>* AddListener(Clazz* object, Function function)
	{
		return eventsContext->AddListener<T>(object, function);
	}

	template<typename T>
	void RemoveListener(EventDelegate<T>* delegate)
	{
		eventsContext->RemoveListener<T>(delegate);
	}

	template<typename T>
	void DispatchMessage(T message)
	{
		eventsContext->DispatchMessage<T>(message);
	}
#pragma endregion

	void RunFrame(float deltaTime);


	class ResourceManager* GetResourceManager();
	class PhysicsSystem* GetPhysicsSystem();
	class AudioEngine* GetAudioEngine();

private:

	void ProcessPhysics(float deltaTime);
	void DrawWorld();
	void ProcessDebug(float deltaTime);

	class RenderingSystem* renderingSystem;
	class PhysicsSystem* physicsSystem;
	class AudioEngine* audioEngine;
	class DebugSystem* debugSystem;

	ECSContext* ecsContext;
	EventsContext* eventsContext;

	class ResourceManager* resourceManager;

	std::vector<Level*> currentlyLoadedLevels;
	std::vector<System*> systems;

};
