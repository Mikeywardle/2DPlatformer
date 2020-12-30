#pragma once

#include "ResourceLibrary.h"
#include "Material.h"

class World;

class MaterialsLibrary : public ResourceLibrary<Material>
{
public:

	MaterialsLibrary(World* world);

	Material* GetMaterial(std::string name);

private:
	static std::string MATERIAL_PATH;

	void LoadMetaData();
	void LoadMaterial(std::string name, std::string path);

	std::unordered_map<std::string, std::string> FileLookup;

	World* world;
};