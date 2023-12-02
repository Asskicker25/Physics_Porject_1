#pragma once

#include "PhysicsShapeAndCollision.h"

class HierarchicalAABBNode
{
private:
	int maxNumOfTriangles = 3;
	int maxDepth = 10;

public:
	int nodeIndex = 0;

	Aabb aabb;

	HierarchicalAABBNode* parentNode = nullptr;
	HierarchicalAABBNode* leftNode;
	HierarchicalAABBNode* rightNode;
	std::vector<int> triangleIndices;

	HierarchicalAABBNode(const Aabb& aabb, const std::vector<Triangle>& triangles,
		std::vector<int> triangleIndices, int nodeIndex, HierarchicalAABBNode* parentNode);
	~HierarchicalAABBNode(); 

	void SplitNode(const std::vector<Triangle>& triangleList);

};

