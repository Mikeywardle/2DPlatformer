#include "ResourceManager.h"

#include <Resources/TextureLibrary.h>
#include <Resources/MeshLibrary.h>
#include <Resources/ShaderLibrary.h>

ResourceManager::ResourceManager()
{
    textureLibrary = new TextureLibrary();
    meshLibrary = new MeshLibrary();
    shaderLibrary = new ShaderLibrary();

}

Mesh* ResourceManager::GetMesh(std::string name)
{
    return meshLibrary->GetMesh(name);
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
    return textureLibrary->GetTexture(name);
}

int ResourceManager::LoadShader(std::string name, std::string vertex, std::string fragment)
{
    return shaderLibrary->LoadShader(name, vertex, fragment);
}

int ResourceManager::GetShader(std::string name)
{
    return shaderLibrary->GetShader(name);
}
