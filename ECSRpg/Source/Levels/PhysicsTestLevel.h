#pragma once

#include <Core/Level.h>
#include <string>

struct Vector3;

class PhysicsTestlevel : public Level
{
public:
	PhysicsTestlevel(World* world) : Level(world) {};

	virtual void LoadLevel() override;
	virtual void OnStart() override;

private:

	void CreatePlayerCamera();
	void CreateMouseSelectionMarker();
	void CreateDirectionalLight(Vector3 rotation);
	Entity CreateMesh(Vector3 position, std::string mesh, std::string material);
	void CreateTile(Vector3 Position, Vector3 Scale, std::string materialName);

};