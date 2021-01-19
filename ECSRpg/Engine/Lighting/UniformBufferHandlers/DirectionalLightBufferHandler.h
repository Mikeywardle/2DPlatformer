#pragma once

class World;

class DirectionalLightBufferHandler
{
public:
	DirectionalLightBufferHandler();

	void BindDirectionalLightData(World* world);

private:
	unsigned int directionalLightIndex;

};

