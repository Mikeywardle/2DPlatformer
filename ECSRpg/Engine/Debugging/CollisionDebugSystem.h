#pragma once

#include "IDebuggingSystem.h"

class CollisionDebugSystem : public IDebuggingSystem
{
public:
	virtual void OnDebugUpdate(World* world, float deltaTime) override;

private:

	void DrawAABBColliders(World* world);
	void DrawSphereColliders(World* world);

private:

	bool renderColliders = false;

	bool renderAABBs = false;
	bool renderSpheres = false;
};

