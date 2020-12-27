#pragma once
#include <ecs/System.h>

struct BillBoardComponent
{
	bool rotateX;
	bool rotateY;
	bool rotateZ;
};

class BillBoardSpriteSystem : public System
{
public:
	BillBoardSpriteSystem(World* world) : System(world) {}

	virtual void OnFrame(float deltaTime) override;
};
