#pragma once

#include <ecs/System.h>

class World;

class SceneTransformSystem
{
public:

	SceneTransformSystem(World* world);

	void Update(const float& deltaTime);

private:

	World* world;
};