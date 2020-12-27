#include "Shader.h"

#include <stdlib.h> 
#include <stdio.h>

#include <Utils/FileIO.h>
#include <glm/gtc/type_ptr.hpp>

#pragma region Error Checking
void CheckShaderCompileErrors(GLuint object)
{
	GLint success;
	GLchar infoLog[1024];

	glGetShaderiv(object, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(object, 1024, NULL, infoLog);
		printf("ERROR::SHADER: Compile-time error:\n");
		printf("%s\n -- --------------------------------------------------- -- \n", infoLog);
	}
}

void CheckProgramCompileErrors(GLuint object)
{
	GLint success;
	GLchar infoLog[1024];

	glGetProgramiv(object, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(object, 1024, NULL, infoLog);
		printf("| ERROR::SHADER: Compile-time error: \n");
		printf("%s\n -- --------------------------------------------------- -- \n", infoLog);
	}
}
#pragma endregion

GLuint CreateShader(const GLchar* shaderSource, GLuint shaderType)
{
	GLuint ID = glCreateShader(shaderType);
	glShaderSource(ID, 1, &shaderSource, NULL);
	glCompileShader(ID);
	CheckShaderCompileErrors(ID);

	return ID;
}

void DeleteShader(GLuint shader)
{
	glDeleteShader(shader);
}

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	CheckProgramCompileErrors(ID);

	return ID;
}

GLuint LoadShaderFromFile(const char* shaderFile, int shaderType)
{
	const GLchar* shaderCode = ReadTextFile(shaderFile);
	GLuint shader =  CreateShader(shaderCode, shaderType);
	delete(shaderCode);
	return shader;
}

void SetShaderFloat(GLuint programID, const GLchar* name, float data)
{
	glUseProgram(programID);
	glUniform1f(glGetUniformLocation(programID, name), data);
}

void SetShaderMatrix4(GLuint programID, const GLchar* name, const glm::mat4& data)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(data));
}

void SetShaderVector3(GLuint programID, const GLchar* name, const Vector3 data)
{
	glUniform3f(glGetUniformLocation(programID, name), data.x, data.y, data.z);
}
