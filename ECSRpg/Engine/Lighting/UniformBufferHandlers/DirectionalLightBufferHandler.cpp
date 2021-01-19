#include "DirectionalLightBufferHandler.h"

#include <Core/World.h>
#include <glad/glad.h>
#include <Rendering/UniformBufferLocations.h>
#include <Maths/Color.h>
#include <Maths/Vector3.h>
#include <Lighting/LightSources/DirectionalLight.h>

struct DirectionalLightData
{
	float strength;
	Vector3 direction;
	Vector3 color;

};

DirectionalLightBufferHandler::DirectionalLightBufferHandler()
{
	glGenBuffers(1, &directionalLightIndex);

	glBindBuffer(GL_UNIFORM_BUFFER, directionalLightIndex);
	glBufferData(GL_UNIFORM_BUFFER, 48, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, DIRECTIONAL_LIGHT_LOCATION);

	glBindBufferRange(GL_UNIFORM_BUFFER, DIRECTIONAL_LIGHT_LOCATION, directionalLightIndex, 0, sizeof(DirectionalLightData));
}

void DirectionalLightBufferHandler::BindDirectionalLightData(World* world)
{
	DirectionalLightData light = DirectionalLightData();

	std::vector<Entity> directionalLights = world->GetEntities<DirectionalLightComponent, Transform>();

	if (directionalLights.size() != 0)
	{
		Entity directionalLight = directionalLights[0];
		DirectionalLightComponent* lightComponent = world->GetComponent<DirectionalLightComponent>(directionalLight);
		Transform* t = world->GetComponent<Transform>(directionalLight);

		Color lightColor = lightComponent->color;
		//light.strength = lightComponent->strength;
		//light.color = Vector3(lightColor.r, lightColor.g, lightColor.b);
		//light.direction = t->GetUp() * -1;
		Vector3 dir = t->GetUp()*-1;

		glBindBuffer(GL_UNIFORM_BUFFER, directionalLightIndex);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &lightComponent->strength);
		glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(Vector3), &dir);
		glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(Vector3), &lightComponent->color);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	//glBindBuffer(GL_UNIFORM_BUFFER, directionalLightIndex);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData), &light);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData), &light);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData), &light);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightData), &light);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
