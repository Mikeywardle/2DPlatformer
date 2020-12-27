#pragma once 
#include <string>

struct Texture2D;
struct Mesh;

class ResourceManager
{
public:
	ResourceManager();

	Mesh* GetMesh(std::string name);

	Texture2D* GetTexture(std::string name);

	int LoadShader(std::string name, std::string vertex, std::string fragment);
	int GetShader(std::string name);

private:
	class TextureLibrary* textureLibrary;
	class ShaderLibrary* shaderLibrary;
	class MeshLibrary* meshLibrary;
};
