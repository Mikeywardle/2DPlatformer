#pragma once

#include <Maths/MathsTypes.h>
#include <Collisions/CollisionDetection.h>
#include <Collisions/RayCasting.h>


#include <vector>
#include <iterator>
#include <algorithm>
#include <set>

#pragma pack(push, 1)
template<typename T>
struct CollisionGridCellCollection
{
public:
	CollisionGridCellCollection();

	void Add(const T& toInsert, T* inPtr);
	void Clear();
	T* GetDataPtr(T* inPtr)const;

	std::vector<T> ToVector(T* inPtr)const;
public:
	unsigned int index : 24 ;
	unsigned int length : 7;
	unsigned int indexSet : 1 ;
};
#pragma pack(pop)

template<typename T>
class CollisionGrid
{
public:

	typedef RaycastingResult(*RaycastingNarrowPhaseCallback)(const Ray ray, const T item, const unsigned int IgnoreEntity, const void* userData);

	void Insert(const T& item);

	void Clear();
	void SetGridDimensions(Vector3 origin, Vector3 gridSize, Vector3 cellSize = Vector3::One);
	void SetDataArrayLength(const int& length);
	void BuildGridCells();

	void Query(const CollisionAABB& QueryAABB, std::vector<T>& results) const;
	void GetAABBClampIndices(const CollisionAABB& queryAABB, Vector3Int& bottomLeft, Vector3Int& topRight) const;
	RaycastingResult CastRay(const Ray ray, const unsigned int IgnoreEntity, RaycastingNarrowPhaseCallback callback, const void* userData) const;

public:

	Vector3 origin;
	Vector3 cellSize;
	Vector3Int dimensions;
	uint32 numberOfCells;

	//AABB vars
	Vector3 centre;
	Vector3 halfLimits;

private:

	bool CoordsInGrid(const Vector3Int coords) const;

	Vector3Int GetCoordinatesFromPosition(const Vector3& position) const;
	Vector3 GetCellPositionFromCoordinates(const Vector3Int& coords) const;
	int GetIndexFromCoords(const Vector3Int& coords) const;

	CollisionAABB GetAABBBounds() const;
private:

	//3D data stored in a 1-D array (the data is an array of T)
	std::vector<CollisionGridCellCollection<uint32>> gridData;
	std::vector<T> dataArray;

	uint32* indicesBuffer = nullptr;
	uint32 indicesBufferSize = 0;
	uint32 currentIndicesBufferIndex = 0;
};

template<typename T>
inline void CollisionGrid<T>::Insert(const T& item)
{
	dataArray.push_back(item);

	const CollisionAABB insertAABB = item.GetAABB();

	Vector3Int bottomLeft;
	Vector3Int topRight;

	GetAABBClampIndices(insertAABB, bottomLeft, topRight);

	for (int z = bottomLeft.z; z <= topRight.z; ++z)
	{
		for (int y = bottomLeft.y; y <= topRight.y; ++y)
		{
			for (int x = bottomLeft.x; x <= topRight.x; ++x)
			{
				const int arrayIndex = GetIndexFromCoords(Vector3Int(x, y, z));

				++gridData[arrayIndex].length;
				++indicesBufferSize;
			}
		}
	}
}

template<typename T>
inline void CollisionGrid<T>::BuildGridCells()
{
	dataArray.shrink_to_fit();

	//allocateBiffer
	if (indicesBuffer == nullptr && indicesBufferSize > 0)
		indicesBuffer = (uint32*)malloc(indicesBufferSize * sizeof(uint32));

	//TODO: Go through every item and insert index into grid buffer
	currentIndicesBufferIndex = 0;

	for (int i = 0; i < dataArray.size(); ++i)
	{
		const T& item = dataArray[i];
		const CollisionAABB insertAABB = item.GetAABB();

		Vector3Int bottomLeft;
		Vector3Int topRight;

		GetAABBClampIndices(insertAABB, bottomLeft, topRight);

		for (int z = bottomLeft.z; z <= topRight.z; ++z)
		{
			for (int y = bottomLeft.y; y <= topRight.y; ++y)
			{
				for (int x = bottomLeft.x; x <= topRight.x; ++x)
				{
					const int arrayIndex = GetIndexFromCoords(Vector3Int(x, y, z));

					CollisionGridCellCollection<uint32>& gridCell = gridData[arrayIndex];
					
					if (!gridCell.indexSet)
					{
						gridCell.index = currentIndicesBufferIndex;
						currentIndicesBufferIndex += gridCell.length;
						gridCell.length = 0;
						gridCell.indexSet = true;
					}

					gridCell.Add(i, indicesBuffer);
				}
			}
		}
	}
}

template<typename T>
inline void CollisionGrid<T>::Query(const CollisionAABB& queryAABB, std::vector<T>& results) const
{
	Vector3Int bottomLeft;
	Vector3Int topRight;
	
	GetAABBClampIndices(queryAABB, bottomLeft,  topRight);

	//Get results in cells AABB hits
	std::vector<int> tempResults;
	for (int z = bottomLeft.z; z <= topRight.z; ++z)
	{
		for (int y = bottomLeft.y; y <= topRight.y; ++y)
		{
			for (int x = bottomLeft.x; x <= topRight.x; ++x)
			{
				const int arrayIndex = GetIndexFromCoords(Vector3Int(x, y, z));
				const CollisionGridCellCollection<uint32>& cell = gridData[arrayIndex];

				if (cell.length > 0)
				{
					std::vector<uint32> cellVector = cell.ToVector(indicesBuffer);
					tempResults.reserve(tempResults.size() + cell.length);
					std::move(cellVector.begin(), cellVector.end(), std::inserter(tempResults, tempResults.end()));
				}
			}
		}
	}

	//Filter out duplicates
	std::set<int> s(tempResults.begin(), tempResults.end());
	tempResults.assign(s.begin(), s.end());

	//Check AABB's are colliding
	for (const int resultIndex : tempResults)
	{
		const T& result = dataArray[resultIndex];
		const CollisionAABB resultAABB = result.GetAABB();
		if (TestAABBvsAABBSimple(resultAABB, queryAABB))
		{
			results.push_back(result);
		}
	}
}

template<typename T>
inline void CollisionGrid<T>::GetAABBClampIndices(const CollisionAABB& queryAABB, Vector3Int& bottomLeft,  Vector3Int& topRight) const
{
	bottomLeft = GetCoordinatesFromPosition(queryAABB.Position - queryAABB.HalfLimits) - Vector3Int(1, 1, 1);
	topRight = GetCoordinatesFromPosition(queryAABB.Position + queryAABB.HalfLimits) + Vector3Int(1, 1, 1);

	//Clamp box to available indices
	bottomLeft.x = fmaxf(bottomLeft.x, 0);
	bottomLeft.y = fmaxf(bottomLeft.y, 0);
	bottomLeft.z = fmaxf(bottomLeft.z, 0);

	topRight.x = fminf(topRight.x, dimensions.x - 1);
	topRight.y = fminf(topRight.y, dimensions.y - 1);
	topRight.z = fminf(topRight.z, dimensions.z - 1);
}

template<typename T>
inline void CollisionGrid<T>::Clear()
{
	for (CollisionGridCellCollection<uint32>& cell : gridData)
	{
		cell.Clear();
	}

	dataArray.clear();

	if (indicesBuffer != nullptr)
		free(indicesBuffer);

	indicesBuffer = nullptr;

	indicesBufferSize = 0;
}

template<typename T>
inline void CollisionGrid<T>::SetGridDimensions(Vector3 origin, Vector3 gridSize, Vector3 cellSize)
{
	const Vector3 fdimensions = gridSize/cellSize;
	this->origin = origin;
	this->dimensions = Vector3Int((int)fdimensions.x, (int)fdimensions.y, (int)fdimensions.z);
	this->cellSize = cellSize;


	this->halfLimits = gridSize / 2.0f;
	this->centre = origin + halfLimits;

	numberOfCells = dimensions.x * dimensions.y * dimensions.z;

	gridData.resize(numberOfCells);
}

template<typename T>
inline void CollisionGrid<T>::SetDataArrayLength(const int& length)
{
	dataArray.reserve(length);
}

template<typename T>
inline CollisionAABB CollisionGrid<T>::GetAABBBounds() const
{
	return CollisionAABB(centre, halfLimits);
}

template<typename T>
inline RaycastingResult CollisionGrid<T>::CastRay(const Ray ray, const unsigned int IgnoreEntity, RaycastingNarrowPhaseCallback callback, const void* userData) const
{
	RaycastingResult result;

	result.hasHit = false; 
	result.Distance = ray.Length;

	float dxdy = ray.Direction.x / ray.Direction.y;
	float dydx = ray.Direction.y / ray.Direction.x;
	float dydz = ray.Direction.y / ray.Direction.z;
	float dzdy = ray.Direction.z / ray.Direction.y;
	float dxdz = ray.Direction.x / ray.Direction.z;
	float dzdx = ray.Direction.z / ray.Direction.x;

	if (ray.Direction.x == 0)
	{
		dzdx = 0;
		dydx = 0;
	}
	if (ray.Direction.y == 0)
	{
		dzdy = 0;
		dxdy = 0;
	}
	if (ray.Direction.z == 0)
	{
		dxdz = 0;
		dydz = 0;
	}

	//Calculate DDA unit vectors
	const Vector3 unitStepSize = Vector3(
		sqrtf(1.0f + (dydx * dydx) + (dzdx * dzdx))
		, sqrtf((dxdy * dxdy) + 1.0f + (dzdy * dzdy))
		, sqrtf((dxdz * dxdz) + (dydz * dydz) + 1.0f)
	);

	Vector3Int worldIndex = GetCoordinatesFromPosition(ray.Start);

	//If raystart isn't in grid, handle this
	if (!CoordsInGrid(worldIndex))
	{
		const CollisionAABB gridAABB = GetAABBBounds();
		const RaycastingResult gridResult = TestRayVsAABB(ray, gridAABB);

		if (gridResult.hasHit)
		{
			const Vector3 newStartPosition = ray.Start + (ray.Direction * (gridResult.Distance + 0.0001f));
			worldIndex = GetCoordinatesFromPosition(newStartPosition);
		}
		else
		{
			//if ray doesn't hit grid exit algorithm
			return result;
		}
	}

	Vector3 rayLength1D;
	Vector3Int stepVector;

	const Vector3 currentCellPosition = GetCellPositionFromCoordinates(worldIndex);
	const Vector3 nextCellPosition = currentCellPosition + cellSize;

	//Initialise vectors
	{
		if (ray.Direction.x < 0)
		{
			stepVector.x = -1;
			rayLength1D.x = (ray.Start.x - currentCellPosition.x) * unitStepSize.x;
		}
		else
		{
			stepVector.x = 1;
			rayLength1D.x = (nextCellPosition.x + 1 - ray.Start.x) * unitStepSize.x;
		}

		if (ray.Direction.y < 0)
		{
			stepVector.y = -1;
			rayLength1D.y = (ray.Start.y - currentCellPosition.y) * unitStepSize.y;
		}
		else
		{
			stepVector.y = 1;
			rayLength1D.y = (nextCellPosition.y + 1 - ray.Start.y) * unitStepSize.y;
		}

		if (ray.Direction.z < 0)
		{
			stepVector.z = -1;
			rayLength1D.z = (ray.Start.z - currentCellPosition.z) * unitStepSize.z;
		}
		else
		{
			stepVector.z = 1;
			rayLength1D.z = (nextCellPosition.z + 1 - ray.Start.z) * unitStepSize.z;
		}
	}

	float rayDistance = 0;
	bool traversing = false;

	std::set<int> testedItems;

	//Move through world to find hit block
	while (!traversing)
	{
		if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z)
		{
			worldIndex.x += stepVector.x;
			rayDistance += fabsf(rayLength1D.x);
			rayLength1D.x += unitStepSize.x;
		}
		else if (rayLength1D.y < rayLength1D.z)
		{
			worldIndex.y += stepVector.y;
			rayDistance += fabsf(rayLength1D.y);
			rayLength1D.y += unitStepSize.y;
		}
		else
		{
			worldIndex.z += stepVector.z;
			rayDistance += fabsf(rayLength1D.z);
			rayLength1D.z += unitStepSize.z;
		}

		//If outside of grid or longer than query ray return 
		if (rayDistance > ray.Length
			|| !CoordsInGrid(worldIndex))
		{
			traversing = true;
		}
		else
		{
			const int worldIndexInt = GetIndexFromCoords(worldIndex);
			const CollisionGridCellCollection<uint32>& hitData = gridData[worldIndexInt];

			const uint32* hitDataPtr = hitData.GetDataPtr(indicesBuffer);
			//if grid box has objects then do raycasting
			for (int i = 0; i<hitData.length; ++i)
			{
				const int& itemIndex = hitDataPtr[i];

				if (testedItems.find(itemIndex) == testedItems.end())
				{
					const T& item = dataArray[itemIndex];
					const CollisionAABB itemAABB = item.GetAABB();

					const RaycastingResult Broadphaseresult = TestRayVsAABB(ray, itemAABB);

					if (Broadphaseresult.hasHit)
					{
						const RaycastingResult callbackResult = callback(ray, item, IgnoreEntity, userData);

						if (callbackResult.hasHit)
						{
							if (!result.hasHit || result.Distance > callbackResult.Distance)
							{
								result = callbackResult;
							}
						}
						testedItems.insert(itemIndex);
					}
				}
			}
		}
	}

	return result;
}

template<typename T>
inline bool CollisionGrid<T>::CoordsInGrid(Vector3Int coords) const
{
	return coords.x >= 0 && coords.x < dimensions.x
		&& coords.y >= 0 && coords.y < dimensions.y
		&& coords.z >= 0 && coords.z < dimensions.z;
}

template<typename T>
inline Vector3Int CollisionGrid<T>::GetCoordinatesFromPosition(const Vector3& position) const
{
	const Vector3 floatCoords = (position - origin) / cellSize;
	return Vector3Int((int)floatCoords.x, (int)floatCoords.y, (int)floatCoords.z);
}

template<typename T>
inline Vector3 CollisionGrid<T>::GetCellPositionFromCoordinates(const Vector3Int& coords) const
{
	const Vector3 floatCoords = Vector3(coords.x, coords.y, coords.z);
	return origin + (floatCoords * cellSize);
}

template<typename T>
inline int CollisionGrid<T>::GetIndexFromCoords(const Vector3Int& coords) const
{
	return coords.x + (dimensions.x * coords.y) + (dimensions.x * dimensions.y * coords.z);
}

template<typename T>
inline CollisionGridCellCollection<T>::CollisionGridCellCollection()
{
	length = 0;
	indexSet = false;
}

template<typename T>
inline void CollisionGridCellCollection<T>::Add(const T& toInsert, T* inPtr)
{	
	T* data = GetDataPtr(inPtr);
	data[length] = toInsert;
	++length;
}

template<typename T>
inline void CollisionGridCellCollection<T>::Clear()
{
	length = 0;
	indexSet = false;
}

template<typename T>
inline T* CollisionGridCellCollection<T>::GetDataPtr(T* inPtr) const
{
	return &(inPtr[index]);
}

template<typename T>
inline std::vector<T> CollisionGridCellCollection<T>::ToVector(T* inPtr) const
{
	const T* data = GetDataPtr(inPtr);

	return std::vector<T>(data, data+length);
}
