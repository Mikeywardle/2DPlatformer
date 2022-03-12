#include "DebugDraws.h"

#include <vector>
#include <math.h>

#include <Maths/Vector3.h>
#include <Maths/Trigonometry.h>

#include <Resources/Shader.h>

const char* DebugDraws::debugVertexShader = 
		"#version 450 core\n"
		"layout(location = 0) in vec3 position;"

		"layout(std140) uniform CameraMatrices"
		"{"
			"mat4 projection;"
			"mat4 view;"
			"mat4 transformlessView;"
		"};"

		"void main()"
		"{"
			"gl_Position = projection * view * vec4(position, 1.0);"
		"}";

const char* DebugDraws::debugFragmentShader =
		"#version 450 core\n"
		"out vec4 FragColor;"

		"void main()"
		"{"
		"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
		"} ";

unsigned int DebugDraws::debugShader = 0;



void DebugDraws::InitialiseDebugShader()
{
	unsigned int frag = CreateShader(debugFragmentShader, GL_FRAGMENT_SHADER);
	unsigned int vert = CreateShader(debugVertexShader, GL_VERTEX_SHADER);

	debugShader = CreateProgram(vert, frag);
}

void DebugDraws::DrawDebugLine(Vector3 start, Vector3 end)
{
}

void DebugDraws::DrawDebugSphere(const Vector3 centre, const float radius, const int numberOfSections)
{
	//Generate Sphere
	std::vector<Vector3> points;
	std::vector<unsigned int> indices;

	for (unsigned int x = 0; x <= numberOfSections; ++x)
	{
		for (unsigned int y = 0; y <= numberOfSections; ++y)
		{
			float xSegment = (float)x / (float)numberOfSections;
			float ySegment = (float)y / (float)numberOfSections;
			float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
			float yPos = cos(ySegment * PI);
			float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);

			points.push_back((Vector3(xPos, yPos, zPos)* radius) + centre);
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < numberOfSections; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= numberOfSections; ++x)
			{
				indices.push_back(y * (numberOfSections + 1) + x);
				indices.push_back((y + 1) * (numberOfSections + 1) + x);
			}
		}
		else
		{
			for (int x = numberOfSections; x >= 0; --x)
			{
				indices.push_back((y + 1) * (numberOfSections + 1) + x);
				indices.push_back(y * (numberOfSections + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	//Draw Sphere
	GLuint sphereVAO;
	glGenVertexArrays(1, &sphereVAO);

	GLuint vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vector3), &points[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDisable(GL_CULL_FACE);
	glUseProgram(debugShader);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


	//Clean up
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glEnable(GL_CULL_FACE);
}

void DebugDraws::DrawDebugCube(Vector3 centre, Vector3 halfLimits)
{
	GLuint cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
}
