#include <Game.h>

#include <Core/World.h>
#include <Levels/DungeonLevel.h>
#include <Levels/MainMenuLevel.h>
#include <Levels/PhysicsTestLevel.h>

#include "Source/Config/TestInputConfig.h"
#include <Config/NeOniPhysicsConfig.h>

#include <Debug/DebugSystemsSetup.h>

int i = 0;
struct foobar
{
	foobar()
	{
		printf("Construct %i", i);
		++i;
	};

	float a;
	int b;
	bool c;
	bool d;
};

int main()
{
	GameContext game = GameContext(TestInputConfig, NeOniPhysicsConfig);

	World* world = game.GetGameWorld();
	world->SwitchLevel<MainMenuLevel>();

	SetupDebugSystems(world);

	return game.StartGame();
}