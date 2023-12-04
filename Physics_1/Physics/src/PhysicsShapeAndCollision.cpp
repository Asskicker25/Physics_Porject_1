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

	if (triangleIndices.empty()) return false;

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

bool CollisionMeshVsMesh(HierarchicalAABBNode* mesh1, HierarchicalAABBNode* mesh2, 
	const glm::mat4 transformMatrix1, const glm::mat4 transformMatrix2, 
	const std::vector<Triangle>& triangles1, const std::vector<Triangle>& triangles2, 
	std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals)
{
	std::set<int> triangleIndices1;
	std::set<int> triangleIndices2;

	CollisionAABBvsHAABB(mesh1->GetModelAABB(), mesh2, triangleIndices1);
	CollisionAABBvsHAABB(mesh2->GetModelAABB(), mesh1, triangleIndices2);

	if (triangleIndices1.empty() || triangleIndices2.empty()) return false;

	int iterationCount = 0;

	for (int i : triangleIndices1)
	{
		for (int j : triangleIndices2)
		{
			Triangle triangle1; //j Loop Triangle of mesh1
			Triangle triangle2; //i Loop Triangle of mesh2

			triangle1.v1 = transformMatrix1 * glm::vec4(triangles1[j].v1, 1.0f);
			triangle1.v2 = transformMatrix1 * glm::vec4(triangles1[j].v2, 1.0f);
			triangle1.v3 = transformMatrix1 * glm::vec4(triangles1[j].v3, 1.0f);
			triangle1.normal = transformMatrix1 * glm::vec4(triangles1[j].normal, 0.0f);

			triangle2.v1 = transformMatrix2 * glm::vec4(triangles2[i].v1, 1.0f);
			triangle2.v2 = transformMatrix2 * glm::vec4(triangles2[i].v2, 1.0f);
			triangle2.v3 = transformMatrix2 * glm::vec4(triangles2[i].v3, 1.0f);
			triangle2.normal = transformMatrix2 * glm::vec4(triangles2[i].normal, 0.0f);

			iterationCount++;

			glm::vec3 collisionPt;
			if (CollisionTriangleVsTriangle(triangle1, triangle2, collisionPt))
			{
				collisionPoints.push_back(collisionPt);
				collisionNormals.push_back(triangle2.normal);
			}

		}

	}

	std::cout << "Size 1 : " << triangleIndices1.size() << " Size 2 :  " << triangleIndices2.size() << std::endl;

	std::cout << "Collision Pt Size : " << collisionPoints.size()<< " , " << collisionNormals.size() << std::endl;
	std::cout << "Iteration Count : " << iterationCount << std::endl;
	return true;
	
}
