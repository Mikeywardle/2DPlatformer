#include "MaterialsLibrary.h"
#include <Utils/XMLReader.h>
#include "ResourceManager.h"
#include <Core/World.h>

std::string MaterialsLibrary::MATERIAL_PATH = "Materials";

void MaterialsLibrary::LoadMetaData()
{
	std::string materialPath = "Resources\\" + MATERIAL_PATH + "\\_Materials.xml";
	xml_document* metaDoc = XMLReader::parseXml(materialPath);

	for (xml_node materialNode = metaDoc->child("Materials").child("Material"); materialNode; materialNode = materialNode.next_sibling())
	{
		std::string materialpath = materialNode.child("file").first_child().value();
		std::string materialname = materialNode.child("name").first_child().value();

		FileLookup[materialname] = materialpath;
	}

	delete metaDoc;
}

void MaterialsLibrary::LoadMaterial(std::string name, std::string path)
{
	std::string materialPath = "Resources\\" + MATERIAL_PATH + "\\" + path;
	xml_document* materialDoc = XMLReader::parseXml(materialPath);
	xml_node rootNode = materialDoc->child("Material");

	ResourceManager* resourceManager = world->GetResourceManager();

	std::string vertex = rootNode.child("Vertex").first_child().value();
	std::string frag = rootNode.child("Fragment").first_child().value();

	int vertexShader = resourceManager->GetShader(vertex);
	int fragShader = resourceManager->GetShader(frag);

	Material toAdd = Material(vertexShader, fragShader);

	//Colors
	for (xml_node colorNode = rootNode.child("Colors").child("Color"); colorNode; colorNode = colorNode.next_sibling())
	{
		std::string colorname = colorNode.child("name").first_child().value();

		float r = atof(colorNode.child("r").first_child().value());
		float g = atof(colorNode.child("g").first_child().value());
		float b = atof(colorNode.child("b").first_child().value());
		float a = atof(colorNode.child("a").first_child().value());

		Color color = Color(r, g, b, a);

		toAdd.SetColor(colorname, color);

	}


	//Textures

	//floats

	resourceTable[name] = toAdd;

	delete materialDoc;
}

MaterialsLibrary::MaterialsLibrary(World* world)
{
	this->world = world;

	LoadMetaData();
}

Material* MaterialsLibrary::GetMaterial(std::string name)
{
	if (resourceTable.find(name) == resourceTable.end())
		LoadMaterial(name, FileLookup[name]);

	return &resourceTable[name];
}
