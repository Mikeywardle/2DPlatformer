#pragma once

class GameWindow;
class World;

struct InputConfiguration;


class GameContext
{
public:
	GameContext(InputConfiguration InputConfig);
	~GameContext();

	int StartGame();
	void EndGame();

	GameWindow* GetGameWindow() const;
	World* GetGameWorld() const;

private:
	void UpdateFrameTime();

private:
	GameWindow* gameWindow;
	World* gameWorld;

	float LastDeltaTime = 0.018f;
	double GameLifetime = 0.0;
	bool ShouldClose = false;
};


