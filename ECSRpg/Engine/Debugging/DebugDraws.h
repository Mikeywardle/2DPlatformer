#pragma once

struct Vector3;

class DebugDraws
{

public:
	//InitialiseShaders
	static void InitialiseDebugShader();

	//Draw Functions
	static void DrawDebugLine(Vector3 start, Vector3 end);
	static void DrawDebugSphere(const Vector3 centre, const float radius, const int numberOfSections);
	static void DrawDebugCube(Vector3 centre, Vector3 halfLimits);

private:

	//Debug Shaders
	static const char* debugVertexShader;
	static const char* debugFragmentShader;

	static unsigned int debugShader;
};
