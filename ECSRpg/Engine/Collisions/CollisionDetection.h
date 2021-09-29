#pragma once

#include "CollisionResult.h"
#include "CollisionShapes.h"
#include "RayCasting.h"

//2D AABB tests
bool TestAABB2DvsAABB2DSimple(const CollisionAABB2D& A, const CollisionAABB2D& B);
bool TestAABB2DContainsAABB2D(const CollisionAABB2D& Container, const CollisionAABB2D& Containee);

//Test Shapes against self
CollisionResult TestABBvAABB(const CollisionAABB& A, const CollisionAABB& B);
bool TestAABBvsAABBSimple(const CollisionAABB& A, const CollisionAABB& B);
CollisionResult TestSpherevsSphere(const CollisionSphere& A, const CollisionSphere& B);

//TEst Shapes against each other
CollisionResult TestSphereVsAABB(const CollisionSphere& sphere, const CollisionAABB& box);

//Ray Functions
RaycastingResult TestRayVsSphere(const Ray& ray, const CollisionSphere& sphere);
RaycastingResult TestRayVsAABB(const Ray& ray, const CollisionAABB& box);