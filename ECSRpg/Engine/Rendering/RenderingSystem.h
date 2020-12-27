#pragma once

#include <vector>

class World;

class RenderingSystem
{
public:

	RenderingSystem(World* world);
	~RenderingSystem();

	void DrawWorld();

private:

	std::vector<class IRenderer*> renderers;

	World* world;
};
