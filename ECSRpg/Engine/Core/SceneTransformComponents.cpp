#include "SceneTransformComponents.h"

#include <math.h>

#include <Maths/Trigonometry.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <Utils/STLUtils.h>

Vector3 SceneTransformNode::GetForward() const
{
    const Vector3 rot = GetRotation();

    const float yRads = DegreesToRadians(rot.y);
    const float zRads = DegreesToRadians(rot.z);

    const float cosZ = cos(zRads);

    Vector3 forward;
    forward.x = cos(yRads) * cosZ;
    forward.y = sin(zRads);
    forward.z = sin(yRads) * cosZ;

    return Vector3::Normalize(forward);
}

Vector3 SceneTransformNode::GetRight() const
{
    //const Vector3 rot = GetRotation();

    //const float yRads = DegreesToRadians(rot.y);
    //const float xRads = DegreesToRadians(rot.x);

    //Vector3 right;
    //right.x = cos(yRads);
    //right.y = cos(xRads);
    //right.z = sin(xRads) * sin(yRads);

    //return Vector3::Normalize(right);

    return Vector3::Normalize(Vector3::CrossProduct(GetForward(), GetUp()));
}

Vector3 SceneTransformNode::GetUp() const
{
    const Vector3 rot = GetRotation();

    const float xRads = DegreesToRadians(rot.x);
    const float yRads = DegreesToRadians(rot.y);
    const float zRads = DegreesToRadians(rot.z);

    const float sinX = sin(xRads);
    const float sinZ = sin(zRads);
    const float sinY = sin(yRads);
    const float cosY = cos(yRads);
    Vector3 up;

    up.x = (sinX * cosY) * (sinZ * sinY);
    up.y = cos(xRads) * cos(zRads);
    up.z = (sinX * sinY) * (sinZ * cosY);

    return Vector3::Normalize(up);
}

bool SceneTransformNode::IsStatic() const
{
    return isStatic;
}

void SceneTransformNode::SetStatic(bool isStatic)
{
    this->isStatic = isStatic;
}

Vector3 SceneTransformNode::TransformPoint(Vector3 point)
{
    glm::vec4 vector4 = glm::vec4(point.x, point.y, point.z, 1);

    if (modelDirty)
    {
        model = CreateModelMatrix(position, rotation);
        modelDirty = false;
    }

    vector4 = model * vector4;

    return Vector3(vector4.x, vector4.y, vector4.z);
}

glm::mat4 SceneTransformNode::GetTransformMatrix()
{
    if (modelDirty)
    {
        model = CreateModelMatrix(position, rotation);
        modelDirty = false;
    }

    if (!parent)
    {
        return model;
    }
    else
    {
        return parent->GetTransformMatrix() * model;
    }


}

glm::mat4 SceneTransformNode::CreateModelMatrix(Vector3 pos, Vector3 rot)
{
    glm::mat4 toReturn = glm::mat4(1.0f);
    toReturn = glm::translate(toReturn, glm::vec3(pos.x, pos.y, pos.z));

    toReturn = glm::rotate(toReturn, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return toReturn;
}

glm::mat4 SceneTransformNode::CreateModelMatrix(Vector3 pos, Vector3 rot, Vector3 scal)
{
    glm::mat4 toReturn = glm::mat4(1.0f);
    toReturn = glm::translate(toReturn, glm::vec3(pos.x, pos.y, pos.z));

    toReturn = glm::rotate(toReturn, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    toReturn = glm::scale(toReturn, glm::vec3(scal.x, scal.y, scal.z));

    return toReturn;
}

void SceneTransformNode::ExtractTransformFromMatrix(glm::mat4 model, Vector3& position, Vector3& rotation, Vector3& scale)
{
    glm::vec3 scal;
    glm::quat rot;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scal, rot, translation, skew, perspective);

    rot = glm::conjugate(rot);

    position = Vector3(translation.x, translation.y, translation.z);
    glm::vec3 eulerRot = glm::eulerAngles(rot);
    rotation = Vector3(RadiansToDegrees(eulerRot.x), RadiansToDegrees(eulerRot.y), RadiansToDegrees(eulerRot.z)) * -1;
    scale = Vector3(scal.x, scal.y, scal.z);
}

void SceneTransformNode::SetParent(SceneTransformNode* parent)
{
    this->parent = parent;
}

void SceneTransformNode::AddChild(SceneTransformNode* child)
{
    children.push_back(child);
}

void SceneTransformNode::RemoveChild(SceneTransformNode* toRemove)
{
    int index = 0;
    for (SceneTransformNode* child : children)
    {
        if (child == toRemove)
        {
            children.erase(children.begin() + index);
        }

        ++index;
    }
}

void SceneTransformNode::OnParentDestroyed()
{
    parent = nullptr;

    NotifyDestroyed();
}

void SceneTransformNode::NotifyDestroyed()
{
    if (!toBeCleared)
    {
        toBeCleared = true;

        for (SceneTransformNode* child : children)
        {
            child->NotifyDestroyed();
        }
    }
}

bool SceneTransformNode::IsToBeCleared() const
{
    return toBeCleared;
}

SceneTransformNode::SceneTransformNode(Vector3 position, Vector3 rotation, bool isStatic)
{
    this->position = position;
    this->rotation = rotation;
    this->isStatic = isStatic;
}

SceneTransformNode::~SceneTransformNode()
{
    if (parent)
    {
        parent->RemoveChild(this);
    }

    for (SceneTransformNode* child : children)
    {
        child->OnParentDestroyed();
    }
}

Vector3 SceneTransformNode::GetPosition() const
{
    if (!parent)
    {
        return GetLocalPosition();
    }
    else
    {
        return parent->TransformPoint(GetLocalPosition());
    }
}

Vector3 SceneTransformNode::GetRotation() const
{
    if (!parent)
    {
        return GetLocalRotation();
    }
    else
    {
        return parent->GetRotation() + GetLocalRotation();
    }
}

void SceneTransformNode::SetPosition(Vector3 inPosition)
{
    if (!parent)
    {
        SetLocalPosition(inPosition);
    }
    else
    {
        glm::vec4 vector4 = glm::vec4(inPosition.x, inPosition.y, inPosition.z, 1);
        const glm::mat4 inverseParent = glm::inverse(parent->GetTransformMatrix());

        vector4 = inverseParent * vector4;

        SetLocalPosition(Vector3(vector4.x, vector4.y, vector4.z));
    }
}

void SceneTransformNode::SetRotation(Vector3 inRotation)
{
    if (!parent)
    {
        SetLocalRotation(inRotation);
    }
    else
    {
        SetLocalRotation(inRotation - parent->GetRotation());
    }
}

Vector3 SceneTransformNode::GetLocalPosition() const
{
    return position;
}

Vector3 SceneTransformNode::GetLocalRotation() const
{
    return rotation;
}

void SceneTransformNode::SetLocalPosition(Vector3 position)
{
    this->position = position;

    modelDirty = true;
}

void SceneTransformNode::SetLocalRotation(Vector3 rotation)
{
    this->rotation = rotation;

    modelDirty = true;
}

void SceneTransformNode::AddTranslation(Vector3 translation)
{
    position += translation;
    modelDirty = true;
}

void SceneTransformNode::AddRotation(Vector3 deltaRotation)
{
    rotation += deltaRotation;
    modelDirty = true;
}

SceneTransformComponent::SceneTransformComponent()
{
    ownedNode = std::make_shared<SceneTransformNode>();
}

Vector3 SceneTransformComponent::GetPosition() const
{
    return ownedNode->GetPosition();
}

Vector3 SceneTransformComponent::GetRotation() const
{
    return ownedNode->GetRotation();
}

void SceneTransformComponent::SetPosition(Vector3 position)
{
    ownedNode->SetPosition(position);
}

void SceneTransformComponent::SetRotation(Vector3 rotation)
{
    ownedNode->SetRotation(rotation);
}

Vector3 SceneTransformComponent::GetLocalPosition() const
{
    return ownedNode->GetLocalPosition();
}

Vector3 SceneTransformComponent::GetLocalRotation() const
{
    return ownedNode->GetLocalRotation();
}

void SceneTransformComponent::SetLocalPosition(Vector3 position)
{
    ownedNode->SetLocalPosition(position);
}

void SceneTransformComponent::SetLocalRotation(Vector3 rotation)
{
    ownedNode->SetLocalRotation(rotation);
}

void SceneTransformComponent::AddTranslation(Vector3 translation)
{
    ownedNode->AddTranslation(translation);
}

void SceneTransformComponent::AddRotation(Vector3 deltaRotation)
{
    ownedNode->AddRotation(deltaRotation);
}

Vector3 SceneTransformComponent::GetForward() const
{
    return ownedNode->GetForward();
}

Vector3 SceneTransformComponent::GetRight() const
{
    return ownedNode->GetRight();
}

Vector3 SceneTransformComponent::GetUp() const
{
    return ownedNode->GetUp();
}

bool SceneTransformComponent::IsStatic() const
{
    return ownedNode->IsStatic();
}

void SceneTransformComponent::SetStatic(bool isStatic)
{
    ownedNode->SetStatic(isStatic);
}

bool SceneTransformComponent::IsToBeDestroyed() const
{
    return ownedNode->IsToBeCleared();
}

Vector3 SceneTransformComponent::TransformPoint(Vector3 point)
{
    return ownedNode->TransformPoint(point);
}

glm::mat4 SceneTransformComponent::GetTransformMatrix()
{
    return ownedNode->GetTransformMatrix();
}

SceneTransformNode* SceneTransformComponent::GetNode() const
{
    return ownedNode.get();
}

void SceneTransformComponent::AttachChild(SceneTransformComponent* child)
{
    SceneTransformNode* childNode = child->GetNode();

    ownedNode->AddChild(childNode);
    childNode->SetParent(ownedNode.get());
}
