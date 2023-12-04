#pragma once

#include "HierarchicalAABBNode.h"

class PhysicsObject;

class HierarchicalAABB
{
private:

	PhysicsObject* phyObj = nullptr;
	int maxDepth = 0;
	std::vector<Triangle> transformedTriangles;

public:
	HierarchicalAABBNode* rootNode = nullptr;

	HierarchicalAABB(PhysicsObject* phyObj, int maxDepth);
	void Construct();

};

