#pragma once

#include "CollisionResult.h"
#include "CollisionShapes.h"
#include "RayCasting.h"

//Test Shapes against self
CollisionResult TestABBvAABB(const CollisionAABB& A, const CollisionAABB& B);
bool TestAABBvsAABBSimple(const CollisionAABB& A, const CollisionAABB& B);
CollisionResult TestSpherevsSphere(const CollisionSphere& A, const CollisionSphere& B);

//Test Shapes against each other
CollisionResult TestSphereVsAABB(const CollisionSphere& sphere, const CollisionAABB& box);

//Ray Functions
RaycastingResult TestRayVsSphere(const Ray& ray, const CollisionSphere& sphere);
RaycastingResult TestRayVsAABB(const Ray& ray, const CollisionAABB& box);