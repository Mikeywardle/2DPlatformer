#pragma once

#include <Events/Event.h>
#include <ecs/Entity.h>
#include <Core/SceneTransformComponents.h>
#include <vector>

struct InputData;
class Level
{
public:
	Level(class World* world);
	~Level();

	virtual void LoadLevel() = 0;
	virtual void OnStart();
	virtual void OnInput(float deltaTime, const InputData* inputData) {};
	virtual void OnFrame(float deltaTime);
	virtual void OnDestroy();


	class World* GetWorld();

	template<typename Clazz, typename Function>
	EventDelegate<float>* AddFrameCallBack(Clazz* object, Function function)
	{
		return onFrameEvent.AddListener(object, function);
	}

	void RemoveFrameCallback(EventDelegate<float>* toRemove);

protected:

	class World* world;
	bool isInitialized = false;

	Event<float> onFrameEvent;
	std::vector<Entity> levelEntities;
};
