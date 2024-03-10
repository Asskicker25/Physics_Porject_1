#pragma once

#include "PhysicsObject.h"
#include "Softbody/BaseSoftBody.h"
#include <Windows.h>

class PhysicsEngine
{
private:
	float timer = 0;
	
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<glm::vec3> collisionPoints;
	std::vector<Model*> debugSpheres;

	std::vector<BaseSoftBody*> listOfSoftBodies;


	void UpdatePhysics(float deltaTime);
 	
public:
	float fixedStepTime = 0.01f;
	glm::vec3 gravity = glm::vec3(0, -9.8f / 3.0f, 0);

	CRITICAL_SECTION* softBody_CritSection = nullptr;

	static PhysicsEngine& GetInstance();

	void AddPhysicsObject(PhysicsObject* physicsObject);
	void RemovePhysicsObject(PhysicsObject* physicsObject);
	bool PhysicsObjectExists(PhysicsObject* physicsObject);
	bool HandleCollision(PhysicsObject* first, PhysicsObject* second,
		std::vector<glm::vec3>& collisionPoint,
		std::vector<glm::vec3>& collisionNormal);

	void AddSoftBodyObject(BaseSoftBody* softBody);
	void RemoveSoftBodyObject(BaseSoftBody* softBody);

	void Update(float deltaTime);
	void UpdateSoftBodies(float deltaTime);
	void UpdateSoftBodyBufferData();
	void SetDebugSpheres(Model* model, int count);


	void Shutdown();
};



static bool RayCast(const glm::vec3& rayOrigin, glm::vec3 rayDir, PhysicsObject* phyObject,
	float rayDistance, glm::vec3& collisionPt, glm::vec3& collisionNormal)
{
	switch (phyObject->shape)
	{
	case SPHERE:
		return RayCastSphere(rayOrigin, rayDir, dynamic_cast<Sphere*>(phyObject->GetTransformedPhysicsShape()),
			rayDistance, collisionPt, collisionNormal);
	case AABB:
		return RayCastAABB(rayOrigin, rayDir, phyObject->GetModelAABB(),
			rayDistance, collisionPt, collisionNormal);
	case MESH_OF_TRIANGLES:
		return RayCastMesh(rayOrigin, rayDir, phyObject->transform.GetTransformMatrix(),
			rayDistance, phyObject->GetTriangleList(), collisionPt, collisionNormal);
	}
	return false;
}