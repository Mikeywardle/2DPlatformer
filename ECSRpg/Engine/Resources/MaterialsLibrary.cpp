#include "MaterialsLibrary.h"
#include <Utils/XMLReader.h>
#include "ResourceManager.h"
#include <Core/World.h>

#include <Rendering/UniformBufferLocations.h>
#include "Texture2D.h"

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

	Material* toAdd = new Material(vertexShader, fragShader);

	//Colors
	for (xml_node colorNode = rootNode.child("Colors").child("Color"); colorNode; colorNode = colorNode.next_sibling())
	{
		const std::string colorname = colorNode.child("name").first_child().value();

		const float r = atof(colorNode.child("r").first_child().value());
		const float g = atof(colorNode.child("g").first_child().value());
		const float b = atof(colorNode.child("b").first_child().value());
		const float a = atof(colorNode.child("a").first_child().value());

		const Color color = Color(r, g, b, a);

		toAdd->SetColor(colorname, color);
	}

	//Textures
	int i = 0;
	for (xml_node textureNode = rootNode.child("Textures").child("Texture"); textureNode; textureNode = textureNode.next_sibling())
	{
		const std::string textureName = textureNode.first_child().value();
		const Texture2D* texture = resourceManager->GetTexture(textureName);
		toAdd->SetTexture(texture->ID, i);

		++i;
	}
	//floats

	//BindUniforms
	unsigned int toAddID = toAdd->ID;
	BindUniforms(toAddID);

	resourceTable[name] = toAdd;

	delete materialDoc;
}

void MaterialsLibrary::BindUniforms(unsigned int toAddID)
{
	//Camera Matrices
	BindUniform(toAddID, CAMERA_MATRICES_LOCATION, CAMERA_MATRICES_NAME);
	BindUniform(toAddID, EYE_POSITION_LOCATION, EYE_POSITION_NAME);

	//Misc info

	//Directional Light
	BindUniform(toAddID, DIRECTIONAL_LIGHT_LOCATION, DIRECTIONAL_LIGHT_NAME);

	//Point Lights

	//Spot lights
}

void MaterialsLibrary::BindUniform(unsigned int toAddID, unsigned int location, const char* name)
{
	unsigned int UniformBlock = glGetUniformBlockIndex(toAddID, name);
	glUniformBlockBinding(toAddID, UniformBlock, location);
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

	return resourceTable[name];
}

void MaterialsLibrary::ClearAssets()
{
	for (std::unordered_map<std::string, Material*>::iterator p = resourceTable.begin();
		p != resourceTable.end(); p++)
	{
		glDeleteProgram(p->second->ID);
		delete(p->second);
	}
	resourceTable.clear();
}
