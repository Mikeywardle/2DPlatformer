#pragma once
#include <Resources/Mesh.h>
#include "ResourceLibrary.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class MeshLibrary : public ResourceLibrary<Mesh>
{
public:
	MeshLibrary();
	void LoadMeshFromFile(std::string fileName);
	Mesh* GetMesh(std::string name);

private:
	static std::string MESH_PATH;

	std::unordered_map<std::string, std::string> FileLookupMap;

	void LoadMetaFile();
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
};
