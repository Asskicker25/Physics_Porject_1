#include "PhysicsShapeAndCollision.h"
#include "HierarchicalAABBNode.h"

void CollisionAABBvsHAABB(const Aabb& sphereAabb, HierarchicalAABBNode* rootNode, std::set<int>& triangleIndices)
{
	if (CollisionAABBvsAABB(sphereAabb, rootNode->GetModelAABB()))
	{
		if (rootNode->triangleIndices.empty())
		{
			if (rootNode->leftNode != nullptr)
			{
				CollisionAABBvsHAABB(sphereAabb, rootNode->leftNode, triangleIndices);
				CollisionAABBvsHAABB(sphereAabb, rootNode->rightNode, triangleIndices);
			}
		}
		else
		{
			triangleIndices.insert(rootNode->triangleIndices.begin(), rootNode->triangleIndices.end());
		}
	}
}

bool CollisionSphereVsMeshOfTriangles(const Aabb& sphereAabb, Sphere* sphere, HierarchicalAABBNode* rootNode, 
	const glm::mat4 transformMatrix, const std::vector<Triangle>& triangles,
	std::vector<glm::vec3>& collisionPoints,
	std::vector<glm::vec3>& collisionNormals)
	

{

	std::set<int> triangleIndices;

	CollisionAABBvsHAABB(sphereAabb, rootNode, triangleIndices);

	if (triangleIndices.empty()) return false;

	for (int i : triangleIndices)
	{
		glm::vec3 collisionPt;

		Triangle triangle;

		triangle.v1 = transformMatrix * glm::vec4(triangles[i].v1, 1.0f);
		triangle.v2 = transformMatrix * glm::vec4(triangles[i].v2, 1.0f);
		triangle.v3 = transformMatrix * glm::vec4(triangles[i].v3, 1.0f);
		triangle.normal = transformMatrix * glm::vec4(triangles[i].normal, 0.0f);

		if (CollisionSphereVsTriangle(sphere, triangle, collisionPt))
		{
			collisionPoints.push_back(collisionPt);
			collisionNormals.push_back(triangle.normal);
		}
	}

	return true;

}

bool CollisionAABBVsMeshOfTriangles(const Aabb& aabb, HierarchicalAABBNode* rootNode, const glm::mat4 transformMatrix, const std::vector<Triangle>& triangles, std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals)
{

	std::set<int> triangleIndices;

	CollisionAABBvsHAABB(aabb, rootNode, triangleIndices);

	for (int i : triangleIndices)
	{
		glm::vec3 collisionPt;

		Triangle triangle;

		triangle.v1 = transformMatrix * glm::vec4(triangles[i].v1, 1.0f);
		triangle.v2 = transformMatrix * glm::vec4(triangles[i].v2, 1.0f);
		triangle.v3 = transformMatrix * glm::vec4(triangles[i].v3, 1.0f);
		triangle.normal = transformMatrix * glm::vec4(triangles[i].normal, 0.0f);

		if (CollisionAABBVsTriangle(aabb, triangle, collisionPt))
		{
			collisionPoints.push_back(collisionPt);
			collisionNormals.push_back(triangle.normal);
		}
	}

	return true;
}
