#pragma once
#include "ResourceLibrary.h"
#include <Resources/Shader.h>

class ShaderLibrary : public ResourceLibrary<GLuint>
{
public:
	GLuint LoadShader(std::string name, std::string vertex, std::string fragment);
	GLuint GetShader(std::string name);

private:
	static std::string SHADER_PATH;
};

