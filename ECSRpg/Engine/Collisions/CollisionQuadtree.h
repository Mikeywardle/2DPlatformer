#pragma once

#include <Maths/MathsTypes.h>
#include <Collisions/CollisionDetection.h>

#include <vector>

#define MAX_DEPTH 8

template<typename T>
struct CollisionQuadtreeNode
{
public:

	~CollisionQuadtreeNode<T> ()
	{
		delete(Children[0]);
		delete(Children[1]);
		delete(Children[2]);
		delete(Children[3]);
	};

	std::vector<T> data;

	//TopLeft, TopRight, BottomLeft, BottomRight
	CollisionQuadtreeNode<T>* Children[4] = { nullptr ,nullptr ,nullptr ,nullptr };

	bool IsLeaf();
	void ClearNode();

};

template<typename T>
class CollisionQuadtree
{
public:
	void Insert(const T& item);
	void Query(const T &QueryData, std::vector<T>& results);
	void Clear();

public:
	Vector2 halfLimits = Vector2::Zero;
	Vector2 position = Vector2::Zero;

private:
	//Insert Functions
	void CheckChildrenAndInsertRecursive(CollisionQuadtreeNode<T>* Node, const T& item, const Vector2& nodePosition, const Vector2& nodeHalfLimits);
	bool FindAppropriateChildNode(CollisionQuadtreeNode<T>* Node, const T& item, const Vector2& nodePosition, const Vector2& nodeHalfLimits);

	//Query Functions
	void RecursivelyQueryNodesvAABB(const CollisionQuadtreeNode<T>* Node, const T& QueryData, std::vector<T>& results, const Vector2& nodePosition, const Vector2& nodeHalfLimits) const;

	//All Helpers
	CollisionAABB2D GetChildAABB(const int& child, const Vector2& nodePosition, const Vector2& childHalfLimits) const;


private:
	CollisionQuadtreeNode<T> RootNode;
};


#pragma region function implementations
template<typename T>
inline bool CollisionQuadtreeNode<T>::IsLeaf()
{
	return Children[0] == nullptr
		&& Children[1] == nullptr
		&& Children[2] == nullptr
		&& Children[3] == nullptr;
}

template<typename T>
inline void CollisionQuadtreeNode<T>::ClearNode()
{
	delete(Children[0]);
	delete(Children[1]);
	delete(Children[2]);
	delete(Children[3]);

	Children[0] = nullptr;
	Children[1] = nullptr;
	Children[2] = nullptr;
	Children[3] = nullptr;

	data.clear();
}

template<typename T>
inline void CollisionQuadtree<T>::Insert(const T& item)
{
	CheckChildrenAndInsertRecursive(&RootNode, item, position, halfLimits);
}

template<typename T>
inline void CollisionQuadtree<T>::Query(const T& queryData, std::vector<T>& results)
{
	const CollisionAABB2D rootAABB = CollisionAABB2D(position, halfLimits);
	const bool collideWithTopLevel = TestAABB2DvsAABB2DSimple(rootAABB, queryData.GetAABB2D());

	if (collideWithTopLevel)
	{
		RecursivelyQueryNodesvAABB(&RootNode, queryData, results, position, halfLimits);
	}
}

template<typename T>
inline void CollisionQuadtree<T>::Clear()
{
	Vector2 halfLimits = Vector2::Zero;
	Vector2 position = Vector2::Zero;

	RootNode.ClearNode();
}

template<typename T>
inline void CollisionQuadtree<T>::CheckChildrenAndInsertRecursive(CollisionQuadtreeNode<T>* Node, const T& item, const Vector2& nodePosition, const Vector2& nodeHalfLimits)
{
	if (Node->IsLeaf() && Node->data.size() == 0)
	{
		Node->data.push_back(item);
		return;
	}

	const bool AddedToChild = FindAppropriateChildNode(Node, item, nodePosition, nodeHalfLimits);

	//If node 1 data point then there is a chance that the child could be added to other nodes
	//If more than 1 data then all the data has likely already been tested on previous asserts
	if (Node->data.size() == 1)
	{
		const T& dat = Node->data[0];
		const bool dataAdded = FindAppropriateChildNode(Node, dat, nodePosition, nodeHalfLimits);

		if (dataAdded)
		{
			Node->data.clear();
		}
	}

	if(!AddedToChild)
	{
		Node->data.push_back(item);
	}

}
template<typename T>
inline bool CollisionQuadtree<T>::FindAppropriateChildNode(CollisionQuadtreeNode<T>* Node, const T& item, const Vector2& nodePosition, const Vector2& nodeHalfLimits)
{
	//Split node into children
	const Vector2 childHalfLimits = nodeHalfLimits / 2.0f;

	const CollisionAABB2D itemAABB = item.GetAABB2D();

	//Calculate which node item is in
	bool isBottom = itemAABB.Position.y < nodePosition.y;
	bool isRight = itemAABB.Position.x > nodePosition.x;

	const int potentialNodeIndex = isRight + (2*isBottom);

	const CollisionAABB2D childAABB = GetChildAABB(potentialNodeIndex, nodePosition, childHalfLimits);

	if (TestAABB2DContainsAABB2D(childAABB, itemAABB))
	{
		if (Node->Children[potentialNodeIndex] == nullptr)
		{
			Node->Children[potentialNodeIndex] = new CollisionQuadtreeNode<T>();
		}
		CheckChildrenAndInsertRecursive(Node->Children[potentialNodeIndex], item, childAABB.Position, childHalfLimits);
		return true;
	}

	return false;
}
template<typename T>
inline void CollisionQuadtree<T>::RecursivelyQueryNodesvAABB(const CollisionQuadtreeNode<T>* Node, const T& QueryData, std::vector<T>& results, const Vector2& nodePosition, const Vector2& nodeHalfLimits) const
{
	const CollisionAABB QueryAABB = QueryData.GetAABB();
	const CollisionAABB2D QueryAABB2D = QueryData.GetAABB2D();

	for (const T data : Node->data)
	{
		if (TestAABBvsAABBSimple(QueryAABB, data.GetAABB()))
			results.push_back(data);
	}

	const Vector2 childHalfLimits = nodeHalfLimits / 2.0f;

	const Vector2 topLeftPos = nodePosition + Vector2(-childHalfLimits.x, childHalfLimits.y);
	const Vector2 topRightPos = nodePosition + (childHalfLimits);
	const Vector2 bottomLeftPos = nodePosition - (childHalfLimits);
	const Vector2 bottomRightPos = nodePosition + Vector2(childHalfLimits.x, -childHalfLimits.y);

	const CollisionAABB2D topLeftAABB = CollisionAABB2D(topLeftPos, childHalfLimits);
	const CollisionAABB2D topRightAABB = CollisionAABB2D(topRightPos, childHalfLimits);
	const CollisionAABB2D bottomLeftAABB = CollisionAABB2D(bottomLeftPos, childHalfLimits);
	const CollisionAABB2D bottomRightAABB = CollisionAABB2D(bottomRightPos, childHalfLimits);

	if (Node->Children[0] != nullptr && TestAABB2DvsAABB2DSimple(QueryAABB2D, topLeftAABB))
	{
		RecursivelyQueryNodesvAABB(Node->Children[0], QueryData, results, topLeftPos, childHalfLimits);
	}
	if (Node->Children[1] != nullptr && TestAABB2DvsAABB2DSimple(QueryAABB2D, topRightAABB))
	{
		RecursivelyQueryNodesvAABB(Node->Children[1], QueryData, results, topRightPos, childHalfLimits);
	}	
	if (Node->Children[2] != nullptr && TestAABB2DvsAABB2DSimple(QueryAABB2D, bottomLeftAABB))
	{
		RecursivelyQueryNodesvAABB(Node->Children[2], QueryData, results, bottomLeftPos, childHalfLimits);
	}	
	if (Node->Children[3] != nullptr && TestAABB2DvsAABB2DSimple(QueryAABB2D, bottomRightAABB))
	{
		RecursivelyQueryNodesvAABB(Node->Children[3], QueryData, results, bottomRightPos, childHalfLimits);
	}
}
template<typename T>
inline CollisionAABB2D CollisionQuadtree<T>::GetChildAABB(const int& child, const Vector2& nodePosition, const Vector2& childHalfLimits) const
{
	Vector2 childPos;

	switch (child)
	{
	case 0:
		childPos = nodePosition + Vector2(-childHalfLimits.x, childHalfLimits.y);
		break;
	case 1:
		childPos = nodePosition + (childHalfLimits);
		break;
	case 2:
		childPos = nodePosition - (childHalfLimits);
		break;
	case 3:
		childPos = nodePosition + Vector2(childHalfLimits.x, -childHalfLimits.y);
		break;
	}

	return CollisionAABB2D(childPos, childHalfLimits);

}
#pragma endregion
