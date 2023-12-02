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
		if (i == 0)
		{
			Debugger::Print("OG : ", triangle.v1);
		}
		triangle.v1 = transformedMatrix * glm::vec4(triangle.v1, 1.0f);
		triangle.v2 = transformedMatrix * glm::vec4(triangle.v2, 1.0f);
		triangle.v3 = transformedMatrix * glm::vec4(triangle.v3, 1.0f) ;
		if (i == 0)
		{
			Debugger::Print("NEW : ", triangle.v1);
		}

		transformedTriangles.push_back(triangle);
	}

	rootNode = new HierarchicalAABBNode(phyObj->GetModelAABB(), transformedTriangles, {}, 0, true, nullptr);
}
