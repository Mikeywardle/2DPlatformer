#pragma once

#include "../IRenderer.h"

class MeshRenderer : public IRenderer
{
	virtual void Draw(World* world) override;
};