#include "RenderingSystem.h"

#include "IRenderer.h"
#include "Sprites/SpriteRenderer.h"
#include "Meshes/MeshRenderer.h"

#include <Core/World.h>

RenderingSystem::RenderingSystem(World* world)
{
	this->world = world;

	renderers = 
	{ 
		new SpriteRenderer() ,
		new MeshRenderer()
	};
}

RenderingSystem::~RenderingSystem()
{
	for (IRenderer* renderer : renderers)
	{
		delete(renderer);
	}
}

void RenderingSystem::DrawWorld()
{
	for (IRenderer* renderer : renderers)
	{
		renderer->Draw(world);
	}
}
