#pragma once

#include <Resources/Mesh.h>
#include <Resources/Material.h>

struct MeshComponent
{
public:
	void SetMesh(Mesh* mesh);

	Mesh* GetMesh() const;
	int GetMeshVAO() const;
	int GetNumberOfIndices() const;


	void SetMaterial(Material* material);
	Material* GetMaterial() const;
	int GetMaterialIndex() const;

protected:

	Mesh* mesh;
	int MeshVAO;
	int NumberOfIndices;



	Material* material;
	int MaterialIndex;
};


