#pragma once

#include <Core/Level.h>

class MainMenuLevel final : public Level
{
public:
	MainMenuLevel(World* world);

	virtual void LoadLevel() override;
	virtual void OnStart() override;
	virtual void OnInput(float deltaTime, const InputData* inputData) override;
};
