#include "Transform.h"
#include <math.h>

#include <Maths/Trigonometry.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <Utils/STLUtils.h>

Vector3 Transform::GetForward() const
{
    float yRads = DegreesToRadians(rotation.y);
    float zRads = DegreesToRadians(rotation.z);

    Vector3 forward;
    forward.x = cos(yRads) * cos(zRads);
    forward.y = sin(zRads);
    forward.z = sin(yRads) * cos(zRads);

    return Vector3::Normalize(forward);
}

Vector3 Transform::GetRight() const
{
    return Vector3::Normalize(Vector3::CrossProduct(GetForward(),Vector3::Up));
}

Vector3 Transform::GetUp() const
{
    return Vector3::Normalize(Vector3::CrossProduct(GetRight(), GetForward()));
}

bool Transform::IsStatic() const
{
    return isStatic;
}

void Transform::SetStatic(bool isStatic)
{
    this->isStatic = isStatic;
}

Vector3 Transform::TransformPoint(Vector3 point)
{
    glm::vec4 vector4 = glm::vec4(point.x, point.y, point.z, 1);

    if (modelDirty)
    {
        model = CreateModelMatrix(position, rotation, scale);
        modelDirty = false;
    }

    vector4 = model * vector4;

    return Vector3(vector4.x, vector4.y, vector4.z);
}

glm::mat4 Transform::GetModel()
{
    // If dirty calculate new Model
    if (modelDirty)
    {
        model =  CreateModelMatrix(position, rotation, scale);
        modelDirty = false;
    }

    return model;
}

glm::mat4 Transform::CreateModelMatrix(Vector3 pos, Vector3 rot, Vector3 scal)
{
    glm::mat4 toReturn = glm::mat4(1.0f);
    toReturn = glm::translate(toReturn, glm::vec3(pos.x, pos.y, pos.z));

    toReturn = glm::rotate(toReturn, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    toReturn = glm::rotate(toReturn, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    toReturn = glm::scale(toReturn, glm::vec3(scal.x, scal.y, scal.z));

    return toReturn;
}

void Transform::ExtractTransformFromMatrix(glm::mat4 model, Vector3& position, Vector3& rotation, Vector3& scale)
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
    rotation = Vector3(RadiansToDegrees(eulerRot.x), RadiansToDegrees(eulerRot.y), RadiansToDegrees(eulerRot.z)) *-1;
    scale = Vector3(scal.x, scal.y, scal.z);
}

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale, bool isStatic)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->isStatic = isStatic;
}

Vector3 Transform::GetPosition() const
{
    return position;
}

Vector3 Transform::GetRotation() const
{
    return rotation;
}

Vector3 Transform::GetScale() const
{
    return scale;
}

void Transform::SetPosition(Vector3 position)
{
    this->position = position;

    modelDirty = true;
}

void Transform::SetRotation(Vector3 rotation)
{
    this->rotation = rotation;

    modelDirty = true;
}

void Transform::SetScale(Vector3 scale)
{
    this->scale = scale;

    modelDirty = true;
}

void Transform::AddTranslation(Vector3 translation)
{
    position += translation;
    modelDirty = true;
}

void Transform::AddRotation(Vector3 deltaRotation)
{
    rotation += deltaRotation;
    modelDirty = true;
}

void Transform::AddScaling(Vector3 deltaScale)
{
    scale += deltaScale;
    modelDirty = true;
}
