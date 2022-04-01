#pragma once

#include <Core/Level.h>
#include <string>

struct Vector3;
class DungeonLevel : public Level
{
public:
	DungeonLevel(World* world) : Level(world) {};

	virtual void LoadLevel() override;
	virtual void OnStart() override;

	virtual void OnInput(float deltaTime, const InputData* inputData) override;

private:

	void CreatePlayerCamera();
	void CreateUnit(Vector3 position);
	void CreateMouseSelectionMarker();
	void CreateFloorSectionMarker(Vector3 position);
	void CreateTile(Vector3 Position, Vector3 Scale, std::string materialName, bool isFloor);
	void CreateDirectionalLight(Vector3 rotation);

	Entity CreateMesh(Vector3 position, std::string mesh, std::string material);

};