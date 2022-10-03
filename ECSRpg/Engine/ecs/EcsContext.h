#pragma once
#include <typeinfo>
#include <map>
#include <stdexcept>

#include "Entity.h"
#include "ComponentBatch.h"
#include "EntitiesManager.h"
#include <functional>

#pragma region N-TypeFunctions
template<typename T1>
void ForEntities_NType(const std::function<void(const Entity, T1*)> f, const std::vector<const IComponentBatch*>& componentBatches, const std::vector<Entity>& entities)
{
	ComponentBatch<T1>* componentBatch = (ComponentBatch<T1>*) componentBatches[0];

	for (const Entity entity : entities)
	{
		f(
			entity
			, componentBatch->GetComponent(entity)
		);
	}

}

template<typename T1, typename T2>
void ForEntities_NType(const std::function<void(const Entity, T1*, T2*)> f, const std::vector<const IComponentBatch*>& componentBatches, const std::vector<Entity>& entities)
{
	ComponentBatch<T1>* componentBatch = (ComponentBatch<T1>*) componentBatches[0];
	ComponentBatch<T2>* componentBatch2 = (ComponentBatch<T2>*) componentBatches[1];

	for (const Entity entity : entities)
	{
		f(
			entity
			, componentBatch->GetComponent(entity)
			, componentBatch2->GetComponent(entity)
		);
	}

}

template<typename T1, typename T2, typename T3>
void ForEntities_NType(const std::function<void(const Entity, T1*, T2*, T3*)> f, const std::vector<const IComponentBatch*>& componentBatches, const std::vector<Entity>& entities)
{
	ComponentBatch<T1>* componentBatch = (ComponentBatch<T1>*) componentBatches[0];
	ComponentBatch<T2>* componentBatch2 = (ComponentBatch<T2>*) componentBatches[1];
	ComponentBatch<T3>* componentBatch3 = (ComponentBatch<T3>*) componentBatches[2];

	for (const Entity entity : entities)
	{
		f(
			entity
			, componentBatch->GetComponent(entity)
			, componentBatch2->GetComponent(entity)
			, componentBatch3->GetComponent(entity)
		);
	}

}

template<typename T1, typename T2, typename T3, typename T4>
void ForEntities_NType(const std::function<void(const Entity, T1*, T2*, T3*, T4*)> f, const std::vector<const IComponentBatch*>& componentBatches, const std::vector<Entity>& entities)
{
	ComponentBatch<T1>* componentBatch = (ComponentBatch<T1>*) componentBatches[0];
	ComponentBatch<T2>* componentBatch2 = (ComponentBatch<T2>*) componentBatches[1];
	ComponentBatch<T3>* componentBatch3 = (ComponentBatch<T3>*) componentBatches[2];
	ComponentBatch<T4>* componentBatch4 = (ComponentBatch<T4>*) componentBatches[3];

	for (const Entity entity : entities)
	{
		f(
			entity
			, componentBatch->GetComponent(entity)
			, componentBatch2->GetComponent(entity)
			, componentBatch3->GetComponent(entity)
			, componentBatch4->GetComponent(entity)
		);
	}

}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
void ForEntities_NType(const std::function<void(const Entity, T1*, T2*, T3*, T4*, T5*)> f, const std::vector<const IComponentBatch*>& componentBatches, const std::vector<Entity>& entities)
{
	ComponentBatch<T1>* componentBatch = (ComponentBatch<T1>*) componentBatches[0];
	ComponentBatch<T2>* componentBatch2 = (ComponentBatch<T2>*) componentBatches[1];
	ComponentBatch<T3>* componentBatch3 = (ComponentBatch<T3>*) componentBatches[2];
	ComponentBatch<T4>* componentBatch4 = (ComponentBatch<T4>*) componentBatches[3];
	ComponentBatch<T5>* componentBatch5 = (ComponentBatch<T5>*) componentBatches[4];

	for (const Entity entity : entities)
	{
		f(
			entity
			, componentBatch->GetComponent(entity)
			, componentBatch2->GetComponent(entity)
			, componentBatch3->GetComponent(entity)
			, componentBatch4->GetComponent(entity)
			, componentBatch5->GetComponent(entity)
		);
	}

}
#pragma endregion

class ECSContext
{
public:
	ECSContext();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void ClearEntities();

	template<typename T>
	T* AddComponent(Entity entity)
	{
		VerifyComponentRegistered<T>();

		ComponentBatch<T>* batch = GetComponentBatch<T>();
		return batch->CreateComponent(entity);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		ComponentBatch<T>* batch = GetComponentBatch<T>();
		batch->RemoveComponent(entity);
	}

	template<typename T>
	T* GetComponent(const Entity entity) const
	{
		return  GetComponentBatch<T>()->GetComponent(entity);
	}

	template<typename T>
	ComponentBatch<T>* GetComponentBatch() const
	{
		const char* hash = typeid(T).raw_name();
		if(componentsLookup.count(hash))
		{
			return (ComponentBatch<T>*)componentsLookup.at(hash);
		}
		else
		{
			return nullptr;
		}
	}

	template<typename... Args>
	void FilterEntities(std::vector<Entity>& entities) const
	{
		std::vector<const char*> types = { (typeid(Args).raw_name())... };
		std::vector<const IComponentBatch*> componentBatches;
		std::vector<Entity> entitiesTemp = std::vector<Entity>();

		//For each type (after type 0) check if current entities also contain those components
		for (int i = 0; i < types.size(); ++i)
		{
			const IComponentBatch* currentTypeBatch = GetComponentBatchFromHash(types[i]);
			if (currentTypeBatch == nullptr)
			{
				return;
			}

			componentBatches.push_back(currentTypeBatch);
		}

		for(const IComponentBatch* currentTypeBatch : componentBatches)
		{
			entitiesTemp = std::vector<Entity>();
			entitiesTemp.reserve(entities.size());

			for (const Entity entity : entities)
			{
				if (currentTypeBatch->ContainsComponent(entity))
					entitiesTemp.push_back(entity);
			}

			entitiesTemp.shrink_to_fit();
			entities = entitiesTemp;
		}
	}

	template<typename T, typename... Args>
	std::vector<Entity> GetEntities() const
	{
		//Get hashes of first type args
		const char* Type0 = typeid(T).raw_name();	
		const IComponentBatch* componentBatch0 = GetComponentBatchFromHash(Type0);

		if (componentBatch0 == nullptr)
		{
			return std::vector<Entity>();
		}

		//Get initial vector of entities from first argument type
		std::vector<Entity> entities = componentBatch0->GetEntitiesWithComponent();

		FilterEntities<Args...>(entities);

		return entities;
	}

	template<typename T>
	std::vector<T>* GetComponents()
	{
		ComponentBatch<T>* batch = GetComponentBatch<T>();

		if (batch != nullptr)
			return batch->GetComponents();
		else
			return nullptr;
	}

	template<typename T>
	bool HasComponent(const Entity entity) const
	{
		const ComponentBatch<T>* batch = GetComponentBatch<T>();

		if (batch == nullptr)
			return false;
		else
			return batch->ContainsComponent(entity);
	}

	template<typename... Args>
	Entity GetEntity() const
	{
		std::vector<Entity> entities = GetEntities<Args...>();

		if (entities.size() == 0)
			return NO_ENTITY;
		else
			return entities[0];
	}

	template<typename T>
	Entity GetComponentOwner(const T* pointer) const
	{
		const ComponentBatch<T>* batch = GetComponentBatch<T>();

		if (batch == nullptr)
			return NO_ENTITY;
		else
			return batch->GetComponentOwner(pointer);
	}


	template<typename... Args>
	void ForEntitiesLambda(const std::function<void(const Entity, Args*...)> f) const
	{
		//Get All Entities
		const std::vector<Entity> entities = GetEntities<Args...>();

		//Get Component Batches
		const std::vector<const char*> types = { (typeid(Args).raw_name())... };
		std::vector<const IComponentBatch*> componentBatches;
		componentBatches.resize(types.size());

		for (int i = 0; i < types.size(); ++i)
		{
			const IComponentBatch* currentTypeBatch = GetComponentBatchFromHash(types[i]);
			if (currentTypeBatch == nullptr)
			{
				return;
			}

			componentBatches[i] = currentTypeBatch;
		}

		//Call Function for
		ForEntities_NType<Args...>(f, componentBatches, entities);
	}

private:

	IComponentBatch* GetComponentBatchFromHash(const char* hash) const
	{
		if (componentsLookup.count(hash))
		{
			return componentsLookup.at(hash);
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	constexpr bool VerifyComponentRegistered()
	{
		const char* hash = typeid(T).raw_name();

		if (componentsLookup.find(hash) == componentsLookup.end())
			componentsLookup[hash] = (IComponentBatch*) new ComponentBatch<T>();

		return true;
	}

	std::map<const char*, IComponentBatch*> componentsLookup;
	EntitiesManager* entitiesManager;
	
};
