#pragma once

#include <vector>
#include <Utils/STLUtils.h>

#include "Entity.h"

class IComponentBatch
{
public:
	virtual bool ContainsComponent(const Entity entity) const = 0;
	virtual std::vector<Entity> GetEntitiesWithComponent() const = 0;
	virtual void RemoveComponent(const Entity entity) = 0;

};

template<typename T>
class ComponentBatch final : public IComponentBatch
{
public:

	ComponentBatch()
	{}

	virtual bool ContainsComponent(const Entity entity) const override
	{
		if (entityToIndexArray.size() <= entity)
		{
			return false;
		}
		else 
		{
			return entityToIndexArray[entity] != 0;
		}

	}

	virtual std::vector<Entity> GetEntitiesWithComponent() const override
	{
		return indexToEntityArray;
	}

	T* CreateComponent(Entity entity)
	{
		int newIndex = componentPackedArray.size();

		componentPackedArray.push_back(T());
		indexToEntityArray.push_back(entity);

		if (entityToIndexArray.size() <= entity)
			entityToIndexArray.resize(entity+1);

		entityToIndexArray[entity] = newIndex+1;

		return &componentPackedArray[newIndex];
	}

	virtual void RemoveComponent(Entity entity) override
	{
		if (!ContainsComponent(entity))
			return;

		unsigned int toRemoveIndex = entityToIndexArray[entity]-1;
		unsigned int lastIndex = componentPackedArray.size()-1;

		componentPackedArray[toRemoveIndex] = componentPackedArray[lastIndex];	

		Entity lastEntity;

		if (lastIndex > 0)
		{
			lastEntity = indexToEntityArray[lastIndex];
			indexToEntityArray[toRemoveIndex] = indexToEntityArray[lastIndex];
		}
		else
		{
			lastEntity = indexToEntityArray[0];
		}

		entityToIndexArray[lastEntity] = toRemoveIndex + 1;
		entityToIndexArray[entity] = 0;


		indexToEntityArray.erase(indexToEntityArray.end()-1);
		componentPackedArray.erase(componentPackedArray.end() - 1);
	}


	T* GetComponent(const Entity entity) const
	{
		if (entity >= entityToIndexArray.size())
			return nullptr;

		const unsigned int index = entityToIndexArray.at(entity);

		if (index == 0)
			return nullptr;
		else
			return (T*)&componentPackedArray.at(index -1);
	}

	std::vector<T>* GetComponents()
	{
		return &componentPackedArray;
	}

	Entity GetEntity(T* pointer) const
	{
		const T* componentsStartPtr = &componentPackedArray[0];
		const int pointerindex = (pointer - componentsStartPtr) / sizeof(T);

		return indexToEntityArray.at(pointerindex);
	}

private:

	//Dense array of components
	std::vector<T> componentPackedArray;

	//Sparse Set to map between Entities and indexes
	std::vector<unsigned int> entityToIndexArray;
	std::vector<Entity> indexToEntityArray;
};



