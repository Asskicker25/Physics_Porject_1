#pragma once

#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	float timer = 0;
	
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<glm::vec3> collisionPoints;
	
	void UpdatePhysics(float deltaTime);

public:
	float fixedStepTime = 0.2f;

	void AddPhysicsObject(PhysicsObject* physicsObject);
	void RemovePhysicsObject(PhysicsObject* physicsObject);
	bool PhysicsObjectExists(PhysicsObject* physicsObject);
	bool HandleCollision(PhysicsObject* first, PhysicsObject* second, glm::vec3& collisionPoint);

	void Update(float deltaTime);
};

