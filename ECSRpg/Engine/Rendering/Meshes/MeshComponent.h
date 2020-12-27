#pragma once

#include <Resources/Mesh.h>

struct MeshComponent
{
public:
	void SetMesh(Mesh* mesh);

	Mesh* GetMesh() const;
	int GetMeshVAO() const;
	int GetNumberOfIndices() const;


	void SetMaterial(int material);
	//Material* GetMaterial
	int GetMaterialIndex() const;

protected:

	Mesh* mesh;
	int MeshVAO;
	int NumberOfIndices;

	//Material Variables

	//Material* material;
	int MaterialIndex;
};


