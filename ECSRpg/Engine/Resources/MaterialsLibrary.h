#pragma once

#include "ResourceLibrary.h"
#include "Material.h"

class World;

class MaterialsLibrary : public ResourceLibrary< Material*>
{
public:

	MaterialsLibrary(World* world);

	Material* GetMaterial(std::string name);
	virtual void ClearAssets() override;

private:
	static std::string MATERIAL_PATH;

	void LoadMetaData();
	void LoadMaterial(std::string name, std::string path);

	void BindUniforms(unsigned int toAddID);
	void BindUniform(unsigned int toAddID, unsigned int location, const char* name);

	std::unordered_map<std::string, std::string> FileLookup;

	World* world;
};