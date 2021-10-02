#pragma once
#include <Maths/Vector3.h>
#include <vector>

#include <glm/mat4x4.hpp>

struct Transform
{
public:
	Transform() = default;
	Transform(Vector3 position, Vector3 rotation= Vector3(), Vector3 scale = Vector3(1,1,1), bool isStatic = false);

	// Getters/Setters
	// Global
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;
	Vector3 GetScale() const;

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 rotation);
	void SetScale(Vector3 scale);

	void AddTranslation(Vector3 translation);
	void AddRotation(Vector3 deltaRotation);
	void AddScaling(Vector3 deltaScale);

	Vector3 GetForward() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;

	bool IsStatic() const;
	void SetStatic(bool isStatic);

	Vector3 TransformPoint(Vector3 point);

	glm::mat4 GetModel();

	//Model Calculation
	static glm::mat4 CreateModelMatrix(Vector3 pos, Vector3 rot, Vector3 scal);
	static void ExtractTransformFromMatrix(glm::mat4 model, Vector3& position, Vector3& rotation, Vector3& scale);

private:

	bool modelDirty = true;
	bool isStatic = false;

	//Global Values
	Vector3 position = Vector3::Zero;
	Vector3 rotation = Vector3::Zero;
	Vector3 scale = Vector3::One;

	glm::mat4 model;
};