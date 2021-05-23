#pragma once
#include <ecs/System.h>

struct BillBoardComponent
{
};

class BillBoardSpriteSystem : public System
{
public:
	BillBoardSpriteSystem(World* world) : System(world) {}

	virtual void OnFrame(float deltaTime) override;
};
