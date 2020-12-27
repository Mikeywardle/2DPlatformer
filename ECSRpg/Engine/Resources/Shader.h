#pragma once
#include <glad/glad.h>
#include<glm/mat4x4.hpp>

#include <Maths/Vector3.h>

#define FRAGMENT_SHADER GL_FRAGMENT_SHADER;
#define VERTEX_SHADER GL_VERTEX_SHADER;

GLuint CreateShader(const GLchar* shaderSource, GLuint shaderType);
void DeleteShader(GLuint shader);

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint LoadShaderFromFile(const GLchar* shaderFile, int shaderType);

void SetShaderFloat(GLuint programID, const GLchar* name, float data);

void SetShaderMatrix4(GLuint programID, const GLchar* name, const glm::mat4& data);

void SetShaderVector3(GLuint programID, const GLchar* name, const Vector3 data);
