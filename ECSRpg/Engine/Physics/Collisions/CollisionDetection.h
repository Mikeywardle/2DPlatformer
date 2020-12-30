#pragma once

#include "CollisionResult.h"

#include "SphereCollider.h"
#include "AABB.h"
#include "RayCasting.h"

CollisionResult TestSphereVsAABB(CollisionSphere& sphere, CollisionAABB& box);

//Ray Functions
RaycastingResult TestRayVsSphere(Ray& ray, CollisionSphere& sphere);
RaycastingResult TestRayVsAABB(Ray& ray, CollisionAABB& box);