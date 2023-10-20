#pragma once

#include <Graphics/Model.h>

#include "PhysicsShape.h"
#include "iPhysicsTransformable.h"


class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	Aabb cachedAABB;
	glm::mat4 cachedMatrix;

public:

	PhysicsShape shape;
	//Since division is slow
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	Aabb aabb;

	PhysicsObject();
	~PhysicsObject();
	
	void Initialize(Model* model);
	Aabb CalculateModelAABB();
	Aabb GetModelAABB();
	
	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override; 

	virtual bool CheckCollision(PhysicsObject* other, glm::vec3& collisionPoint) = 0;

};

//class PhysicsSphere : public PhysicsObject
//{
//
//};