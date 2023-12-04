#include "HierarchicalAABBNode.h"

HierarchicalAABBNode::HierarchicalAABBNode(const Aabb& aabb,
	const std::vector<Triangle>& triangles, std::vector<int> triangleIndices, int nodeIndex,
	HierarchicalAABBNode* parentNode, Model* model, int maxDepth)
	: aabb(aabb), leftNode(nullptr), rightNode(nullptr)
{
	this->aabb = aabb;
	this->nodeIndex = nodeIndex;
	this->parentNode = parentNode;
	this->model = model;
	this->maxDepth = maxDepth;

	if ((int)triangleIndices.size() == 0)
	{
		for (int i = 0; i < (int)triangles.size(); i++)
		{
			this->triangleIndices.push_back(i);
		}

		if ((int)this->triangleIndices.size() > maxNumOfTriangles)
		{
			SplitNode(triangles);
			this->triangleIndices.clear();
			return;
		}
	}

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		int triangleIndex = triangleIndices[i];
		/*if (IsTriangleInsideAABB(triangles[triangleIndex], aabb))
		{
			this->triangleIndices.push_back(triangleIndex);
		}*/
		glm::vec3 clpt;
		if (CollisionAABBVsTriangle(aabb,triangles[triangleIndex], clpt))
		{
			this->triangleIndices.push_back(triangleIndex);
		}
	}

	if (nodeIndex < maxDepth)
	{
		if ((int)this->triangleIndices.size() > maxNumOfTriangles)
		{
			SplitNode(triangles);
			this->triangleIndices.clear();
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

	std::vector<int> leftTriangleIndices;
	std::vector<int> rightTriangleIndices;

	for (int i : triangleIndices)
	{
		leftTriangleIndices.push_back(i);
		rightTriangleIndices.push_back(i);
	}


	leftNode = new HierarchicalAABBNode(leftAABB, triangleList, leftTriangleIndices, (nodeIndex + 1), this, model,maxDepth);
	rightNode = new HierarchicalAABBNode(rightAABB, triangleList, rightTriangleIndices, (nodeIndex + 1), this, model, maxDepth);

	//if (this->triangleIndices.size() > maxNumOfTriangles && nodeIndex < maxDepth)
	//{
	//	//triangleIndices.clear();
	//}

}

Aabb HierarchicalAABBNode::GetModelAABB()
{

	glm::mat4 transformMatrix = model->transform.GetTransformMatrix();

	glm::vec3 originalMinV = aabb.min;
	glm::vec3 originalMaxV = aabb.max;

	glm::vec4 vertices[8];
	vertices[0] = transformMatrix * glm::vec4(originalMinV.x, originalMinV.y, originalMinV.z, 1.0f);
	vertices[1] = transformMatrix * glm::vec4(originalMinV.x, originalMinV.y, originalMaxV.z, 1.0f);
	vertices[2] = transformMatrix * glm::vec4(originalMinV.x, originalMaxV.y, originalMinV.z, 1.0f);
	vertices[3] = transformMatrix * glm::vec4(originalMinV.x, originalMaxV.y, originalMaxV.z, 1.0f);
	vertices[4] = transformMatrix * glm::vec4(originalMaxV.x, originalMinV.y, originalMinV.z, 1.0f);
	vertices[5] = transformMatrix * glm::vec4(originalMaxV.x, originalMinV.y, originalMaxV.z, 1.0f);
	vertices[6] = transformMatrix * glm::vec4(originalMaxV.x, originalMaxV.y, originalMinV.z, 1.0f);
	vertices[7] = transformMatrix * glm::vec4(originalMaxV.x, originalMaxV.y, originalMaxV.z, 1.0f);

	glm::vec3 transformedMinV = glm::vec3(vertices[0]);
	glm::vec3 transformedMaxV = glm::vec3(vertices[0]);

	for (int i = 1; i < 8; ++i) {
		transformedMinV = glm::min(transformedMinV, glm::vec3(vertices[i]));
		transformedMaxV = glm::max(transformedMaxV, glm::vec3(vertices[i]));
	}

	return 	Aabb(transformedMinV, transformedMaxV);
}
