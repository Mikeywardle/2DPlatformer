#pragma once

#include <string>
#include <map>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Maths/Color.h>

struct Material
{
public:
	GLuint ID;
	Material(GLuint vertexShader, GLuint fragmentShader);
	Material() = default;

	void SetFloat(std::string name, float value);
	float GetFloat(std::string name);

	void SetVector3(std::string name, glm::vec3 value);
	glm::vec3 GetVector3(std::string name);

	void SetColor(std::string name, Color value);
	Color GetColor(std::string name);

	void SetMatrix4(const GLchar* name, const glm::mat4& matrix);

	void SetTexture(int id, int index);
	std::vector<int>textureIds;


private:
	std::map<std::string, float> floats;
	std::map<std::string, glm::vec3> vector3s;
	std::map<std::string, Color> colors;

	void checkCompileErrors(GLuint object);
};