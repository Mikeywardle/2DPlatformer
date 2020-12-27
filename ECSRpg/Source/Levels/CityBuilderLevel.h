#pragma once

#include <Core/Level.h>

struct Vector3;

class CityBuilderLevel : public Level
{
public:
	CityBuilderLevel(World* world) : Level(world) {};

	virtual void OnStart() override;

	virtual void OnFrame(float deltaTime) override;

private:
	void CreatePlayerCamera();
	void CreatePlayer();
	void CreateSprite(Vector3 Position);
	Entity CreatePlatform(Vector3 position, float width, float height);
	void CreateFloatingPlatform(Vector3 position, float width, float height, Vector3 start, Vector3 end);
	void CreateCoin(struct Vector3 position);
};
