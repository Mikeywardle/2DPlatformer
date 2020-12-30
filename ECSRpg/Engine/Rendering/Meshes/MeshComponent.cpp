#include "MeshComponent.h"
#include <Core/GameObject.h>

void MeshComponent::SetMesh(Mesh* mesh)
{
    this->mesh = mesh;
    MeshVAO = mesh->VAO;
    NumberOfIndices = mesh->indices.size();
}

Mesh* MeshComponent::GetMesh() const
{
    return mesh;
}

int MeshComponent::GetMeshVAO() const
{
    return MeshVAO;
}

int MeshComponent::GetNumberOfIndices() const
{
    return NumberOfIndices;
}

void MeshComponent::SetMaterial(Material* material)
{
    material = material;
    MaterialIndex = material->ID;
}

Material* MeshComponent::GetMaterial() const
{
    return nullptr;
}

int MeshComponent::GetMaterialIndex() const
{
    return MaterialIndex;
}
