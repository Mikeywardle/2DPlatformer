#pragma once 
#include <string>

struct Texture2D;
struct Mesh;
struct Material;

class World;

class ResourceManager
{
public:
	ResourceManager(World* world);

	Mesh* GetMesh(std::string name);
	Texture2D* GetTexture(std::string name);
	Material* GetMaterial(std::string name);
	int GetShader(std::string name);

	void ClearResources();

private:
	class TextureLibrary* textureLibrary;
	class ShaderLibrary* shaderLibrary;
	class MeshLibrary* meshLibrary;
	class MaterialsLibrary* materialLibrary;
};
