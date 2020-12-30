#include "ResourceManager.h"

#include <Resources/TextureLibrary.h>
#include <Resources/MeshLibrary.h>
#include <Resources/ShaderLibrary.h>
#include <Resources/MaterialsLibrary.h>

ResourceManager::ResourceManager(World* world)
{
    textureLibrary = new TextureLibrary();
    meshLibrary = new MeshLibrary();
    shaderLibrary = new ShaderLibrary();
    materialLibrary = new MaterialsLibrary(world);

}

Mesh* ResourceManager::GetMesh(std::string name)
{
    return meshLibrary->GetMesh(name);
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
    return textureLibrary->GetTexture(name);
}

Material* ResourceManager::GetMaterial(std::string name)
{
    return materialLibrary->GetMaterial(name);
}


int ResourceManager::GetShader(std::string name)
{
    return shaderLibrary->GetShader(name);
}
