#pragma once

#include <vector>
#include <memory>

#include <Maths/Vector3.h>

#include <glm/mat4x4.hpp>

struct SceneTransformNode
{

public:
	SceneTransformNode() = default;
	SceneTransformNode(Vector3 position, Vector3 rotation = Vector3(), bool isStatic = false);

	~SceneTransformNode();

	// Getters/Setters
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 rotation);

	Vector3 GetLocalPosition() const;
	Vector3 GetLocalRotation() const;

	void SetLocalPosition(Vector3 position);
	void SetLocalRotation(Vector3 rotation);

	void AddTranslation(Vector3 translation);
	void AddRotation(Vector3 deltaRotation);

	Vector3 GetForward() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;

	bool IsStatic() const;
	void SetStatic(bool isStatic);

	Vector3 TransformPoint(Vector3 point);

	glm::mat4 GetTransformMatrix();

	//Model Calculation
	static glm::mat4 CreateModelMatrix(Vector3 pos, Vector3 rot);
	static glm::mat4 CreateModelMatrix(Vector3 pos, Vector3 rot, Vector3 scal);
	static void ExtractTransformFromMatrix(glm::mat4 model, Vector3 & position, Vector3 & rotation, Vector3 & scale);

	//Parent Handling
	void SetParent(SceneTransformNode* parent);
	void AddChild(SceneTransformNode* child);
	void RemoveChild(SceneTransformNode* toRemove);
	void OnParentDestroyed();
	void NotifyDestroyed();
	bool IsToBeCleared() const;

private:

	Vector3 position = Vector3::Zero;
	Vector3 rotation = Vector3::Zero;

	glm::mat4 model;

	SceneTransformNode* parent = nullptr;

	std::vector<SceneTransformNode*> children;

	bool modelDirty = true;
	bool isStatic = false;
	bool toBeCleared = false;

};

struct SceneTransformComponent
{
public:

	SceneTransformComponent();

	Vector3 GetPosition() const;
	Vector3 GetRotation() const;

	void SetPosition(Vector3 position);
	void SetRotation(Vector3 rotation);

	Vector3 GetLocalPosition() const;
	Vector3 GetLocalRotation() const;

	void SetLocalPosition(Vector3 position);
	void SetLocalRotation(Vector3 rotation);

	void AddTranslation(Vector3 translation);
	void AddRotation(Vector3 deltaRotation);

	Vector3 GetForward() const;
	Vector3 GetRight() const;
	Vector3 GetUp() const;

	bool IsStatic() const;
	void SetStatic(bool isStatic);
	bool IsToBeDestroyed() const;

	Vector3 TransformPoint(Vector3 point);

	glm::mat4 GetTransformMatrix();

	SceneTransformNode* GetNode() const;

	void AttachChild(SceneTransformComponent* child);

private:
	std::shared_ptr<SceneTransformNode> ownedNode { nullptr };

};
