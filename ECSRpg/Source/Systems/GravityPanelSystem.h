#pragma once

#include <ecs/System.h>

struct TriggerOverlapBeginEvent;

class GravityPanelSystem : public System
{
public:
	GravityPanelSystem(World* world);
	~GravityPanelSystem();

private:
	void OnTriggerOverlap(TriggerOverlapBeginEvent& event);
};
