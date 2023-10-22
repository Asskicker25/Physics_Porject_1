#pragma once

#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	float timer = 0;
	
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<glm::vec3> collisionPoints;
	std::vector<Model*> debugSpheres;
 	
	void UpdatePhysics(float deltaTime);

public:
	float fixedStepTime = 0.2f;
	glm::vec3 gravity = glm::vec3(0, -9.8f / 3.0f, 0);

	void AddPhysicsObject(PhysicsObject* physicsObject);
	void RemovePhysicsObject(PhysicsObject* physicsObject);
	bool PhysicsObjectExists(PhysicsObject* physicsObject);
	bool HandleCollision(PhysicsObject* first, PhysicsObject* second,
		std::vector<glm::vec3>& collisionPoint,
		std::vector<glm::vec3>& collisionNormal);

	void Update(float deltaTime);
	void SetDebugSpheres(Model* model, int count);
};

