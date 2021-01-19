#pragma once

#include <Core/Level.h>
#include <string>

struct Vector3;
class DungeonLevel : public Level
{
public:
	DungeonLevel(World* world) : Level(world) {};

	virtual void OnStart() override;

private:

	void SpawnPlayer();
	void CreatePlayerCamera();
	void CreateTile(Vector3 Position, Vector3 Scale, std::string materialName);
	void CreateDirectionalLight(Vector3 rotation);

	void CreateMesh(Vector3 position, std::string mesh);

};