#pragma once
#include <glad/glad.h>
#include <vector>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>


struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords;
};

struct Mesh
{
	Mesh() {}

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};