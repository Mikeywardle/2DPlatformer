#include "ShaderLibrary.h"


std::string ShaderLibrary::SHADER_PATH = "Shaders";

GLuint ShaderLibrary::LoadShader(std::string name,std::string vertex, std::string fragment)
{
	std::string fragPath = "Resources\\" + SHADER_PATH + "\\" + fragment;
	std::string vertPath = "Resources\\" + SHADER_PATH + "\\" + vertex;

	GLuint Frag = LoadShaderFromFile(fragPath.c_str(), GL_FRAGMENT_SHADER);
	GLuint Vert = LoadShaderFromFile(vertPath.c_str(), GL_VERTEX_SHADER);

	GLuint program = CreateProgram(Vert, Frag);

	resourceTable[name] = program;

	DeleteShader(Frag);
	DeleteShader(Vert);

	return program;
}

GLuint ShaderLibrary::GetShader(std::string name)
{
    return resourceTable[name];
}
