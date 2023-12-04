#include "HierarchicalAABB.h"
#include "PhysicsObject.h"

HierarchicalAABB::HierarchicalAABB(PhysicsObject* phyObj, int maxDepth)
{
	this->phyObj = phyObj;
	this->maxDepth = maxDepth;
	Construct();
}

void HierarchicalAABB::Construct()
{
	rootNode = new HierarchicalAABBNode(phyObj->GetAABB(), phyObj->GetTriangleList(), {}, 0, nullptr,
		phyObj->GetModel(), maxDepth);
}
