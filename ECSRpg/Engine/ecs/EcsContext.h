#pragma once
#include <typeinfo>
#include <map>
#include <initializer_list>

#include "Entity.h"
#include "ComponentBatch.h"
#include "EntitiesManager.h"


class ECSContext
{
public:
	ECSContext();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

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
		return (ComponentBatch<T>*)componentsLookup.at(hash);
	}

	template<typename T>
	ComponentBatch<T>* GetComponentBatchSafe() const
	{
		const char* hash = typeid(T).raw_name();
		if (componentsLookup.find(hash) != componentsLookup.end())
			return (ComponentBatch<T>*)componentsLookup.at(hash);
		else
			return nullptr;
	}

	template<typename... Args>
	void FilterEntities(std::vector<Entity>& entities) const
	{
		std::vector<const char*> types = { (typeid(Args).raw_name())... };
		std::vector<Entity> entitiesTemp = std::vector<Entity>();

		//For each type (after type 0) check if current entities also contain those components
		for (int i = 0; i < types.size(); ++i)
		{
			const IComponentBatch* currentTypeBatch = GetComponentBatchFromHash(types[i]);
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
	std::vector<Entity> GetEntities()
	{
		VerifyComponentsRegistered<T,Args...>();

		//Get hashes of all type args
		const char* Type0 = typeid(T).raw_name();	

		//Get initial vector of entities from first argument type
		std::vector<Entity> entities = GetComponentBatchFromHash(Type0)->GetEntitiesWithComponent();

		FilterEntities<Args...>(entities);

		return entities;
	}

	template<typename T>
	std::vector<T>* GetComponents() 
	{
		ComponentBatch<T>* batch = GetComponentBatchSafe<T>();

		if (batch != nullptr)
			return batch->GetComponents();
		else
			return nullptr;
	}

	template<typename T>
	bool HasComponent(Entity entity) const
	{
		ComponentBatch<T>* batch = GetComponentBatch<T>();

		if (batch == nullptr)
			return false;
		else
			return batch->ContainsComponent(entity);
	}
private:

	IComponentBatch* GetComponentBatchFromHash(const char* hash) const
	{
		return componentsLookup.at(hash);
	}

	template<typename... Args>
	constexpr void VerifyComponentsRegistered()
	{
		std::vector<bool> comps = { (VerifyComponentRegistered<Args>())...};
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
