#pragma once

#include "HierarchicalAABBNode.h"

class PhysicsObject;

class HierarchicalAABB
{
private:

	PhysicsObject* phyObj = nullptr;
	std::vector<Triangle> transformedTriangles;

public:
	HierarchicalAABBNode* rootNode = nullptr;

	HierarchicalAABB(PhysicsObject* phyObj);
	void Construct();

};

