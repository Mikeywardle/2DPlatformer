#include "MainMenuLevel.h"

#include <Inputs/InputData.h>

#include <Core/World.h>

#include "DungeonLevel.h"

#include <Config/TestInputConfig.h>
#include <Inputs/InputValues.h>

MainMenuLevel::MainMenuLevel(World* world)
	:Level(world)
{
}

void MainMenuLevel::LoadLevel()
{
}

void MainMenuLevel::OnStart()
{
	printf("MAIN MENU\nPress \"SPCAEBAR\" to start\n");
}

void MainMenuLevel::OnInput(float deltaTime, const InputData* inputData)
{
	if (inputData->GetInputValue(TestConfigInputId::Jump, InputTypes::BUTTON_PRESSED))
	{
		world->SwitchLevel<DungeonLevel>();
	}
}
