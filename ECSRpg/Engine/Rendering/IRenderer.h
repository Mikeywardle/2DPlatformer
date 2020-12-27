#pragma once

#include <Core/World.h>

class IRenderer
{
public:
	//virtual void CullEntities() = 0;
	virtual void Draw(World* world) = 0;
};
