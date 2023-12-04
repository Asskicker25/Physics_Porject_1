#pragma once


#include <Graphics/Model.h>
#include "PhysicsShapeAndCollision.h"

class HierarchicalAABBNode
{
private:
	int maxNumOfTriangles = 3;
	int maxDepth = 10;
	Model* model;
	Aabb aabb;

public:
	int nodeIndex = 0;


	HierarchicalAABBNode* parentNode = nullptr;
	HierarchicalAABBNode* leftNode;
	HierarchicalAABBNode* rightNode;
	std::vector<int> triangleIndices;

	HierarchicalAABBNode(const Aabb& aabb, const std::vector<Triangle>& triangles,
		std::vector<int> triangleIndices, int nodeIndex, HierarchicalAABBNode* parentNode, Model* model, int maxDepth);
	~HierarchicalAABBNode(); 

	void SplitNode(const std::vector<Triangle>& triangleList);
	Aabb GetModelAABB();

};

