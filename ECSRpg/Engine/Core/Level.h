#pragma once

#include <Events/Event.h>
#include <ecs/Entity.h>
#include "GameObject.h"
#include <Maths/Transform.h>
#include <vector>

class Level
{
public:
	Level(class World* world);
	~Level();

	virtual void OnStart();
	virtual void OnFrame(float deltaTime);
	virtual void OnDestroy();


	class World* GetWorld();

	template<typename Clazz, typename Function>
	EventDelegate<float>* AddFrameCallBack(Clazz* object, Function function)
	{
		return onFrameEvent.AddListener(object, function);
	}

	void RemoveFrameCallback(EventDelegate<float>* toRemove);

public:
	Transform transform;

protected:

	class World* world;
	bool isInitialized = false;

	Event<float> onFrameEvent;
	std::vector<Entity> levelEntities;
};
