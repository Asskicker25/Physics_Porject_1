#pragma once

#include "PhysicsShapeAndCollision.h"
class PhysicsObject;

class HierarchicalAABBNode
{
private:
	int maxNumOfTriangles = 3;
	int maxDepth = 10;
	Model* model = nullptr;
	Aabb aabb;
	PhysicsObject* phyObj;

public:
	int nodeIndex = 0;


	HierarchicalAABBNode* parentNode = nullptr;
	HierarchicalAABBNode* leftNode;
	HierarchicalAABBNode* rightNode;
	std::vector<int> triangleIndices;

	HierarchicalAABBNode(const Aabb& aabb, PhysicsObject* phyObj,
		std::vector<int> triangleIndices, int nodeIndex, HierarchicalAABBNode* parentNode);
	~HierarchicalAABBNode(); 

	void SplitNode();
	Aabb GetAABB();

};

