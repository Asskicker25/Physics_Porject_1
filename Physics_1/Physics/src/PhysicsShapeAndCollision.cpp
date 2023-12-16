#include "PhysicsShapeAndCollision.h"
#include "HierarchicalAABBNode.h"

void CollisionAABBvsHAABB(const Aabb& sphereAabb, HierarchicalAABBNode* rootNode, 
	std::set<int>& triangleIndices, std::vector<Aabb>& collisionAabbs)
{
	if (CollisionAABBvsAABB(sphereAabb, rootNode->GetModelAABB()))
	{
		collisionAabbs.push_back(rootNode->GetModelAABB());

		if (rootNode->triangleIndices.empty())
		{
			if (rootNode->leftNode != nullptr)
			{
				CollisionAABBvsHAABB(sphereAabb, rootNode->leftNode, triangleIndices, collisionAabbs);
				CollisionAABBvsHAABB(sphereAabb, rootNode->rightNode, triangleIndices, collisionAabbs);
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
	std::vector<glm::vec3>& collisionNormals,
	std::vector<Aabb>& collisionAabbs)
	

{
	collisionAabbs.clear();
	std::set<int> triangleIndices;

	CollisionAABBvsHAABB(sphereAabb, rootNode, triangleIndices, collisionAabbs);

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

bool CollisionAABBVsMeshOfTriangles(const Aabb& aabb, HierarchicalAABBNode* rootNode, 
	const glm::mat4 transformMatrix,
	const std::vector<Triangle>& triangles, 
	std::vector<glm::vec3>& collisionPoints, 
	std::vector<glm::vec3>& collisionNormals,
	std::vector<Aabb>& collisionAabbs)
{

	std::set<int> triangleIndices;

	CollisionAABBvsHAABB(aabb, rootNode, triangleIndices, collisionAabbs);

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

void CollisionMeshVsMeshRecursive(HierarchicalAABBNode* mesh1, HierarchicalAABBNode* mesh2,
	std::set<int>& triangleIndices1, std::set<int>& triangleIndices2)
{
	if (CollisionAABBvsAABB(mesh1->GetModelAABB(), mesh2->GetModelAABB()))
	{
		if (mesh1->leftNode != nullptr)
		{
			if (mesh2->leftNode != nullptr)
			{
				CollisionMeshVsMeshRecursive(mesh1->leftNode, mesh2->leftNode, triangleIndices1, triangleIndices2);
				CollisionMeshVsMeshRecursive(mesh1->leftNode, mesh2->rightNode, triangleIndices1, triangleIndices2);

				CollisionMeshVsMeshRecursive(mesh1->rightNode, mesh2->leftNode, triangleIndices1, triangleIndices2);
				CollisionMeshVsMeshRecursive(mesh1->rightNode, mesh2->rightNode, triangleIndices1, triangleIndices2);
			}
			else
			{
				CollisionMeshVsMeshRecursive(mesh1->leftNode, mesh2, triangleIndices1, triangleIndices2);
				CollisionMeshVsMeshRecursive(mesh1->rightNode, mesh2, triangleIndices1, triangleIndices2);
			}
		}
		else
		{
			if (mesh2->leftNode != nullptr)
			{
				CollisionMeshVsMeshRecursive(mesh1, mesh2->leftNode, triangleIndices1, triangleIndices2);
				CollisionMeshVsMeshRecursive(mesh1, mesh2->rightNode, triangleIndices1, triangleIndices2);
			}
			else
			{
				triangleIndices1.insert(mesh1->triangleIndices.begin(), mesh1->triangleIndices.end());
				triangleIndices2.insert(mesh2->triangleIndices.begin(), mesh2->triangleIndices.end());
			}
		}
	}
}


bool CollisionMeshVsMesh(HierarchicalAABBNode* mesh1, HierarchicalAABBNode* mesh2, 
	const glm::mat4 transformMatrix1, const glm::mat4 transformMatrix2, 
	const std::vector<Triangle>& triangles1, const std::vector<Triangle>& triangles2, 
	std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals)
{

	std::set<int> triangleIndices1;
	std::set<int> triangleIndices2;

	CollisionMeshVsMeshRecursive(mesh1, mesh2, triangleIndices1, triangleIndices2);

	if (triangleIndices1.empty() || triangleIndices2.empty()) return false;


	for (int i : triangleIndices1)
	{
		Triangle triangle1 = triangles1[i];
		triangle1.v1 = transformMatrix1 * glm::vec4(triangle1.v1, 1.0f);
		triangle1.v2 = transformMatrix1 * glm::vec4(triangle1.v2, 1.0f);
		triangle1.v3 = transformMatrix1 * glm::vec4(triangle1.v3, 1.0f);
		triangle1.normal = transformMatrix1 * glm::vec4(triangle1.normal, 0.0f);

		for (int j : triangleIndices2)
		{

			Triangle triangle2 = triangles2[j];
			triangle2.v1 = transformMatrix2 * glm::vec4(triangle2.v1, 1.0f);
			triangle2.v2 = transformMatrix2 * glm::vec4(triangle2.v2, 1.0f);
			triangle2.v3 = transformMatrix2 * glm::vec4(triangle2.v3, 1.0f);
			triangle2.normal = transformMatrix2 * glm::vec4(triangle2.normal, 0.0f);

			glm::vec3 collPt;
			if (CollisionTriangleVsTriangle(triangle1, triangle2))
			{
				collisionPoints.push_back(collPt);
				collisionNormals.push_back(triangle2.normal);
			}
		}
	}

	if (collisionPoints.empty()) return false;

	std::cout << "Size 1 : " << triangleIndices1.size() << std::endl;
	std::cout << "Size 2 : " << triangleIndices2.size() << std::endl;
	std::cout << "Collision Pt  : " << collisionPoints.size() << std::endl;


	return true;
}
