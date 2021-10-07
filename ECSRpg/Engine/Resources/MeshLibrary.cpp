#include "MeshLibrary.h"

#include <iostream>
#include <Utils/XMLReader.h>

std::string MeshLibrary::MESH_PATH = "Meshes";

Mesh* MeshLibrary::GetMesh(std::string name)
{
	if (resourceTable.find(name) == resourceTable.end())
		LoadMeshFromFile(FileLookupMap[name]);
	
	return resourceTable[name];
}

void MeshLibrary::ClearAssets()
{
	for (std::unordered_map<std::string, Mesh*>::iterator p = resourceTable.begin();
		p != resourceTable.end(); p++)
	{
		delete(p->second);
	}
	resourceTable.clear();
}

MeshLibrary::MeshLibrary()
{
	LoadMetaFile();
}

void MeshLibrary::LoadMeshFromFile(std::string fileName)
{
	Assimp::Importer importer;
	std::string meshpath = "Resources\\" + MESH_PATH +"\\" + fileName;
	const aiScene* scene = importer.ReadFile(meshpath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error Loading Model" << importer.GetErrorString() << std::endl;
	}
	else
	{
		ProcessNode(scene->mRootNode, scene);
	}
}

void MeshLibrary::LoadMetaFile()
{
	std::string meshpath = "Resources\\" + MESH_PATH + "\\_Meshes.xml";
	xml_document* metaDoc = XMLReader::parseXml(meshpath);

	for (xml_node meshNode = metaDoc->child("Meshes").child("Mesh"); meshNode; meshNode = meshNode.next_sibling())
	{
		std::string meshpath = meshNode.child("file").first_child().value();
		std::string meshname = meshNode.child("name").first_child().value();

		FileLookupMap[meshname] = meshpath;
	}

	delete metaDoc;
}

void MeshLibrary::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//Loading Vertices (positions, normals, texture coordinates)
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		Vector3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			Vector2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = Vector2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	//Loading indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	std::string name(mesh->mName.C_Str());
	resourceTable[name] = new Mesh(vertices, indices);
}

void MeshLibrary::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
