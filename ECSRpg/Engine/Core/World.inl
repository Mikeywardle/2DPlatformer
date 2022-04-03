
#include <ecs/EcsContext.h>
#include "Level.h"
#include <ecs/System.h>

#include <ecs/Entity.h>
#include <Events/EventsContext.h>
#include <thread>
#include "World.h"


#pragma region Levels
//Levels
template<typename T>
inline T* World::SwitchLevel()
{

	if (currentlyLoadedLevel != nullptr)
	{
		delete(currentlyLoadedLevel);

		DeRegisterAllSystems();
		ClearWorld();
	}


	T* newLevel = new T(this);
	currentlyLoadedLevel = dynamic_cast<Level*>(newLevel);

	//TODO: make load level Asynchronous
	currentlyLoadedLevel->LoadLevel();

	BuildWorld();
	currentlyLoadedLevel->OnStart();


	return newLevel;

}
#pragma endregion

#pragma region ECS
template<typename T>
inline T* World::AddComponent(const Entity entity)
{
	return ecsContext->AddComponent<T>(entity);
}

template <typename T>
inline void World::RemoveComponent(const Entity entity)
{
	ecsContext->RemoveComponent<T>(entity);
}

template<typename T>
inline T* World::GetComponent(const Entity entity) const
{
	return ecsContext->GetComponent<T>(entity);
}

template<typename... Args>
inline void World::FilterEntities(std::vector<Entity>& entities) const
{
	ecsContext->FilterEntities<Args...>(entities);
}

template<typename... Args>
inline std::vector<Entity> World::GetEntities() const
{
	return ecsContext->GetEntities<Args...>();
}

template<typename T>
inline bool World::HasComponent(const Entity entity) const
{
	return ecsContext->HasComponent<T>(entity);
}

template<class T>
inline T* World::RegisterSystem()
{
	T* toReturn = new T(this);
	systems.push_back(toReturn);

	return toReturn;
}

template<typename T>
inline std::vector<T>* World::GetComponents()
{
	return ecsContext->GetComponents<T>();
}

template<typename ...Args>
inline Entity World::GetEntity() const
{
	return ecsContext->GetEntity<Args...>();
}
template<typename T>
inline Entity World::GetComponentOwner(const T* pointer) const
{
	return ecsContext->GetComponentOwner<T>(pointer);
}
#pragma endregion

#pragma region Events
//Events Functions
template<typename T, typename Clazz, typename Function>
inline EventDelegate<T>* World::AddListener(Clazz* object, Function function)
{
	return eventsContext->AddListener<T>(object, function);
}

template<typename T>
inline void World::RemoveListener(EventDelegate<T>* delegate)
{
	eventsContext->RemoveListener<T>(delegate);
}

template<typename T>
inline void World::DispatchMessage(T message)
{
	eventsContext->DispatchMessage<T>(message);
}

#pragma endregion

template<typename T>
inline GameState* World::CreateNewGameState()
{
	DeleteGameState();

	gameState = new T();
	return gameState;
}
