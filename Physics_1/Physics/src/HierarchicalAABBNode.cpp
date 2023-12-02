#include "HierarchicalAABBNode.h"
#include "PhysicsObject.h"

HierarchicalAABBNode::HierarchicalAABBNode(const Aabb& aabb, PhysicsObject* phyObj,
	 std::vector<int> triangleIndices, int nodeIndex,
	HierarchicalAABBNode* parentNode)
	: aabb(aabb), leftNode(nullptr), rightNode(nullptr)
{
	this->aabb = aabb;
	this->nodeIndex = nodeIndex;
	this->parentNode = parentNode;
	this->phyObj = phyObj;

	std::vector<Triangle> triangles = phyObj->GetTriangleList();

	if ((int)triangleIndices.size() == 0)
	{
		for (int i = 0; i < triangles.size(); i++)
		{
			this->triangleIndices.push_back(i);
		}

		SplitNode();
		this->triangleIndices.clear();
		return;
	}

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		int triangleIndex = triangleIndices[i];
		/*if (IsTriangleInsideAABB(triangles[triangleIndex], aabb))
		{
			this->triangleIndices.push_back(triangleIndex);
		}*/
		glm::vec3 clpt;
		if (CollisionAABBVsTriangle(aabb, triangles[triangleIndex], clpt))
		{
			this->triangleIndices.push_back(triangleIndex);
		}
	}

	if (nodeIndex < maxDepth)
	{
		if ((int)this->triangleIndices.size() > maxNumOfTriangles)
		{
			SplitNode();
			this->triangleIndices.clear();
		}
	}

}


HierarchicalAABBNode::~HierarchicalAABBNode()
{
	delete leftNode;
	delete rightNode;
}

void HierarchicalAABBNode::SplitNode()
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


	leftNode = new HierarchicalAABBNode(leftAABB, phyObj, leftTriangleIndices, (nodeIndex + 1), this);
	rightNode = new HierarchicalAABBNode(rightAABB, phyObj, rightTriangleIndices, (nodeIndex + 1), this);

	//if (this->triangleIndices.size() > maxNumOfTriangles && nodeIndex < maxDepth)
	//{
	//	//triangleIndices.clear();
	//}

}

Aabb HierarchicalAABBNode::GetAABB()
{
	Aabb localAABB;
	glm::mat4 transformMatrix = model->transform.GetTransformMatrix();
	glm::vec3 minPoint = glm::vec3(transformMatrix * glm::vec4(aabb.min, 1.0f));
	glm::vec3 maxPoint = glm::vec3(transformMatrix * glm::vec4(aabb.max, 1.0f));

	localAABB.min = glm::min(minPoint, maxPoint);
	localAABB.max = glm::max(minPoint, maxPoint);

	return localAABB;
}
