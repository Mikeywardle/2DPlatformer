//#include "CollisionQuadtreeTests.h"
//#include <Collisions/CollisionQuadtree.h>
//#include <Collisions/CollisionDetection.h>
//
//#include <random>
//#include <Analytics/Timer.h>
//
//namespace QuadtreeTests
//{
//	struct TestCollisionData
//	{
//		TestCollisionData(Entity InEntity, Vector2 InPosition, Vector2 InHalfLimits)
//		{
//			entity = InEntity;
//			position = InPosition;
//			halfLimits = InHalfLimits;
//		}
//
//		Entity entity;
//		Vector2 position;
//		Vector2 halfLimits;
//
//		CollisionAABB GetAABB2D() const
//		{
//			return CollisionAABB2D(position, halfLimits);
//		}
//	};
//
//	void QuadtreePerformanceTest(const int& TestWidth, const int& InsertTests, const int& QueryTests)
//	{
//		const int HalfTestWidth = TestWidth / 2.0f;
//
//		CollisionQuadtree<TestCollisionData> quadtree;
//		quadtree.position = Vector2(0, 0);
//		quadtree.halfLimits = Vector2(HalfTestWidth, HalfTestWidth);
//
//
//		std::random_device rd;
//		std::mt19937 mt(rd());
//		std::uniform_real_distribution<float> posdist(-HalfTestWidth, HalfTestWidth);
//		std::uniform_real_distribution<float> widthdist(1, 6);
//
//		Timer timer;
//
//		//Inserts Test
//		std::vector<TestCollisionData> inserts;
//
//		for (int i = 0; i < InsertTests; ++i)
//		{
//			inserts.push_back
//			(
//				TestCollisionData
//				(
//					0
//					, Vector2(posdist(mt), posdist(mt))
//					, Vector2(1, 1)
//				)
//
//			);
//		}
//
//		timer.StartTimer();
//		for (const TestCollisionData col : inserts)
//		{
//			quadtree.Insert(col);
//		}
//		const double insertTime = timer.StopTimer();
//		printf("Time to insert %i objects = %f ms\n", InsertTests, insertTime);
//
//		//Queries Test
//		std::vector<CollisionAABB> queries;
//		for (int i = 0; i < QueryTests; ++i)
//		{
//			queries.push_back
//			(
//				CollisionAABB
//				(
//					Vector2(posdist(mt), posdist(mt))
//					, widthdist(mt)
//					, widthdist(mt)
//				)
//			);
//		}
//
//
//		timer.StartTimer();
//		for (const CollisionAABB col : queries)
//		{
//			std::vector<TestCollisionData> results;
//			quadtree.QueryWithAABB(col, results);
//		}
//		const double queryTime = timer.StopTimer();
//
//		printf("Time to query %i objects with %i colliders = %f ms\n", InsertTests, QueryTests, queryTime);
//
//		//Clear Tree test
//		timer.StartTimer();
//		quadtree.Clear();
//		double clearIime = timer.StopTimer();
//		printf("Time to clear %i objects = %f ms\n", InsertTests, clearIime);
//
//
//		//Brute force test
//		std::vector<bool> results;
//		timer.StartTimer();
//		for (int i = 0; i < InsertTests; ++i)
//		{
//			const CollisionAABB insertAABB = inserts[i].GetAABB();
//			for (int j = 0; j < QueryTests; ++j)
//			{
//				const CollisionAABB queryAABB = queries[j];
//				results.push_back(TestAABBvsAABBSimple(insertAABB, queryAABB));
//			}
//		}
//
//		const double bruteForceTime = timer.StopTimer();
//
//		printf("Time to brute force %i objects with %i colliders = %f ms\n", InsertTests, QueryTests, bruteForceTime);
//	}
//}