#include "ShaderLibrary.h"
#include <Utils/XMLReader.h>


std::string ShaderLibrary::SHADER_PATH = "Shaders";

ShaderLibrary::ShaderLibrary()
{
	LoadMetaData();
}

void ShaderLibrary::LoadShader(std::string name,std::string path, int type)
{
	std::string shaderPath = "Resources\\" + SHADER_PATH + "\\" + path;

	GLuint shader = LoadShaderFromFile(shaderPath.c_str(), type);

	resourceTable[name] = shader;
}

GLuint ShaderLibrary::GetShader(std::string name)
{
	if (resourceTable.find(name) == resourceTable.end())
		LoadShader(name, FileLookup[name].path, FileLookup[name].type);



    return resourceTable[name];
}

void ShaderLibrary::LoadMetaData()
{
	std::string shaderPath = "Resources\\" + SHADER_PATH + "\\_Shaders.xml";
	xml_document* metaDoc = XMLReader::parseXml(shaderPath);

	for (xml_node shaderNode = metaDoc->child("Shaders").child("Shader"); shaderNode; shaderNode = shaderNode.next_sibling())
	{
		std::string shaderfile = shaderNode.child("file").first_child().value();
		std::string shadername = shaderNode.child("name").first_child().value();
		std::string shaderTypeName = shaderNode.child("type").first_child().value();

		int shaderType;

		if (shaderTypeName == "FRAG")
			shaderType = GL_FRAGMENT_SHADER;
		else
			shaderType = GL_VERTEX_SHADER;

		FileLookup[shadername] = ShaderImportData(shaderfile, shaderType);
	}

	delete metaDoc;
}

ShaderLibrary::ShaderImportData::ShaderImportData(std::string path, int type)
{
	this->path = path;
	this->type = type;
}
