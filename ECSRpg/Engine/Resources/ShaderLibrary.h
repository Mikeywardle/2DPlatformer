#pragma once
#include "ResourceLibrary.h"
#include <Resources/Shader.h>

class ShaderLibrary : public ResourceLibrary<GLuint>
{
public:
	ShaderLibrary();
	void LoadShader(std::string name, std::string path, int type);
	GLuint GetShader(std::string name);

private:
	static std::string SHADER_PATH;

	void LoadMetaData();

	struct ShaderImportData
	{
		ShaderImportData() = default;
		ShaderImportData(std::string path, int type);

		std::string path;
		int type;
	};

	std::unordered_map<std::string, ShaderImportData> FileLookup;
};

