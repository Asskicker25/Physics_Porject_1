#pragma once

#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	float timer = 0;
	
	std::vector<PhysicsObject*> physicsObjects;
	
	void UpdatePhysics(float deltaTime);

public:
	float fixedStepTime = 0.2f;

	void AddPhysicsObject(PhysicsObject* physicsObject);
	void RemovePhysicsObject(PhysicsObject* physicsObject);
	bool PhysicsObjectExists(PhysicsObject* physicsObject);

	void Update(float deltaTime);
};

