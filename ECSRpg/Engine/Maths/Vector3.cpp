#include "Vector3.h"
#include <math.h>

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

#pragma region Vector3 Operators
Vector3 Vector3::operator+(const Vector3 b) const
{
    return Vector3(x+b.x, y+b.y, z+b.z);
}

Vector3 Vector3::operator-(const Vector3 b) const
{
    return Vector3(x - b.x, y - b.y, z - b.z);
}

Vector3 Vector3::operator/(const Vector3 b) const
{
    return Vector3(x / b.x, y / b.y, z / b.z);
}

Vector3 Vector3::operator*(const Vector3 b) const
{
    return Vector3(x * b.x, y * b.y, z * b.z);
}

void Vector3::operator+=(const Vector3 b)
{
    x += b.x;
    y += b.y;
    z += b.z;
}

void Vector3::operator-=(const Vector3 b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
}

void Vector3::operator/=(const Vector3 b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;
}

void Vector3::operator*=(const Vector3 b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;
}
#pragma endregion

#pragma region Float Operators
Vector3 Vector3::operator+(const float b) const
{
    return Vector3(x + b, y + b, z + b);
}

Vector3 Vector3::operator-(const float b) const
{
    return Vector3(x - b, y - b, z - b);
}

Vector3 Vector3::operator/(const float b) const
{
    return Vector3(x / b, y / b, z/b);
}

Vector3 Vector3::operator*(const float b) const
{
    return Vector3(x * b, y * b, z * b);
}

void Vector3::operator+=(const float b)
{
    x += b;
    y += b;
    z += b;
}

void Vector3::operator-=(const float b)
{
    x -= b;
    y -= b;
    z -= b;
}

void Vector3::operator/=(const float b)
{
    x /= b;
    y /= b;
    z /= b;
}

void Vector3::operator*=(const float b)
{
    x *= b;
    y *= b;
    z *= b;
}
#pragma endregion

#pragma region Static methods
Vector3 Vector3::CrossProduct(Vector3 a, Vector3 b)
{
    Vector3 c =
    {
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };

    return c;
}

float Vector3::DotProduct(Vector3 a, Vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::Normalize(Vector3 a)
{
    float Magnitude = Vector3::Magnitude(a);
    if (Magnitude == 0)
        return a;

    return a/Magnitude;
}

float Vector3::Magnitude(Vector3 a)
{
    float toRoot = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
    return sqrt(toRoot);
}
Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float alpha)
{
    return (a * (1 - alpha)) + (b * alpha);
}

float Vector3::ReverseLerp(Vector3 a, Vector3 b, Vector3 output)
{
    Vector3 result = (output - a) / (b-a);
    return Vector3::Magnitude(result);
}
#pragma endregion
