#pragma once

#include <Graphics/Model.h>

#include "PhysicsShape.h"
#include "iPhysicsTransformable.h"


class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	AABB cachedAABB;
	glm::mat4 cachedMatrix;

public:

	//Since division is slow
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	AABB aabb;

	PhysicsObject();
	~PhysicsObject();
	
	void Initialize(Model* model);
	AABB CalculateModelAABB();
	AABB GetModelAABB();
	
	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override; 
};

