#include "HierarchicalAABB.h"
#include "PhysicsObject.h"

HierarchicalAABB::HierarchicalAABB(PhysicsObject* phyObj)
{
	this->phyObj = phyObj;
	Construct();
}

void HierarchicalAABB::Construct()
{
	glm::mat4 transformedMatrix = phyObj->GetModel()->transform.GetTransformMatrix();
	for (int i = 0; i < phyObj->GetTriangleList().size(); i++)
	{
		Triangle triangle = phyObj->GetTriangleList()[i];
		
		triangle.v1 = transformedMatrix * glm::vec4(triangle.v1, 1.0f);
		triangle.v2 = transformedMatrix * glm::vec4(triangle.v2, 1.0f);
		triangle.v3 = transformedMatrix * glm::vec4(triangle.v3, 1.0f) ;

		transformedTriangles.push_back(triangle);
	}

	rootNode = new HierarchicalAABBNode(phyObj->GetAABB(), phyObj->GetTriangleList(), {}, 0, nullptr,
		phyObj->GetModel());
}
