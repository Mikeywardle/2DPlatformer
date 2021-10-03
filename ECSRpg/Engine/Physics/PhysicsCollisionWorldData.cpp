#include "PhysicsCollisionWorldData.h"

#include <Collisions/CollisionShapes.h>

PhysicsCollisionWorldData::PhysicsCollisionWorldData(Entity InEntity, Vector3 InPosition, Vector3 InHalfLimits, ColliderType InType, bool isTrigger)
{
    entity = InEntity;
    type = InType;
    IsTrigger = isTrigger;

    BoundingBox = CollisionAABB(InPosition, InHalfLimits);

    const Vector2 position2D = Vector2(InPosition.x, InPosition.z);
    const Vector2 halfLimits2D = Vector2(InHalfLimits.x, InHalfLimits.z);

    BoundingBox2D = CollisionAABB2D(position2D, halfLimits2D);

}

CollisionAABB PhysicsCollisionWorldData::GetAABB() const
{
    return BoundingBox;
}

CollisionAABB2D PhysicsCollisionWorldData::GetAABB2D() const
{
    return BoundingBox2D;
}
