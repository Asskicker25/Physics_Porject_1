#include "HierarchicalAABBNode.h"

HierarchicalAABBNode::HierarchicalAABBNode(const Aabb& aabb,
	const std::vector<Triangle>& triangles, std::vector<int> triangleIndices, int nodeIndex,
	HierarchicalAABBNode* parentNode)
	: aabb(aabb), leftNode(nullptr), rightNode(nullptr)
{
	this->aabb = aabb;
	this->nodeIndex = nodeIndex;
	this->parentNode = parentNode;

	if ((int)triangleIndices.size() == 0)
	{
		for (int i = 0; i < (int)triangles.size(); i++)
		{
			this->triangleIndices.push_back(i);
		}

		SplitNode(triangles);
		return;
	}

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		int triangleIndex = triangleIndices[i];
		if (IsTriangleInsideAABB(triangles[triangleIndex], aabb))
		{
			this->triangleIndices.push_back(triangleIndex);
		}
	}

	if (nodeIndex < maxDepth)
	{
		if ((int)this->triangleIndices.size() > maxNumOfTriangles)
		{
			SplitNode(triangles);
		}
	}

}


HierarchicalAABBNode::~HierarchicalAABBNode()
{
	delete leftNode;
	delete rightNode;
}

void HierarchicalAABBNode::SplitNode(const std::vector<Triangle>& triangleList)
{
	int splitAxis = aabb.GetMaxExtentAxis();
	float splitPosition = 0.5f * (aabb.min[splitAxis] + aabb.max[splitAxis]);

	Aabb leftAABB = aabb;
	leftAABB.max[splitAxis] = splitPosition;

	Aabb rightAABB = aabb;
	rightAABB.min[splitAxis] = splitPosition;

	leftNode = new HierarchicalAABBNode(leftAABB, triangleList, triangleIndices, (nodeIndex + 1), this);
	rightNode = new HierarchicalAABBNode(rightAABB, triangleList, triangleIndices, (nodeIndex + 1), this);

	if (this->triangleIndices.size() > maxNumOfTriangles && nodeIndex < maxDepth)
	{
		//triangleIndices.clear();
	}

}
