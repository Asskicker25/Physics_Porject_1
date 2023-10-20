#pragma once

#include <Graphics/Model.h>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"


class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	Aabb cachedAABB;
	glm::mat4 cachedMatrix;

public:
	PhysicsMode mode = PhysicsMode::STATIC;
	PhysicsShape shape = PhysicsShape::SPHERE;

	//Since division is slow
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	Aabb aabb;
	iShape* physicsShape;

	PhysicsObject();
	~PhysicsObject();
	
	void Initialize(Model* model,PhysicsShape shape, PhysicsMode mode = STATIC);

	Aabb CalculateModelAABB();
	Aabb GetModelAABB();

	Sphere* CalculateModelSphere();

	bool CheckCollision(PhysicsObject* other, glm::vec3& collisionPoint);
	
	
	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override; 

	

};

//class PhysicsSphere : public PhysicsObject
//{
//
//};