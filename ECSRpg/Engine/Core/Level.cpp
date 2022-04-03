#include "Level.h"
#include "World.h"

#include <ecs/EcsContext.h>

#include <Utils/STLUtils.h>

Level::Level(World* world)
{
	this->world = world;

}

Level::~Level()
{

	for (Entity entity : levelEntities)
	{
		world->DestroyEntity(entity);
	}
}

void Level::OnStart()
{
	isInitialized = true;
}

void Level::OnFrame(float deltaTime)
{
	onFrameEvent.Invoke(deltaTime);
}

void Level::OnDestroy()
{
}

World* Level::GetWorld()
{
	return world;
}


void Level::RemoveFrameCallback(EventDelegate<float>* toRemove)
{
	onFrameEvent.RemoveListener(toRemove);
}
