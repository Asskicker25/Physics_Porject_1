#pragma once

#include <Graphics/Model.h>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"


class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	Aabb cachedAABB;
	Aabb aabb;
	glm::mat4 cachedMatrix;

	std::vector < std::vector < Triangle > > triangles;
	std::vector < std::vector<Sphere*> > triangleSpheres;

public:
	PhysicsMode mode = PhysicsMode::STATIC;
	PhysicsShape shape = PhysicsShape::SPHERE;

	//Since division is slow
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f);
	//glm::vec3 oldPosition = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	iShape* physicsShape;
	iShape* transformedPhysicsShape;

	PhysicsObject();
	~PhysicsObject();
	
	void Initialize(Model* model,PhysicsShape shape, PhysicsMode mode = STATIC);

	Aabb CalculateModelAABB();
	Aabb GetModelAABB();

	void CalculatePhysicsShape();
	iShape* GetTransformedPhysicsShape();

	void CalculateTriangleSpheres();

	bool CheckCollision(PhysicsObject* other, glm::vec3& collisionPoint);
	
	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override; 

	


	// Inherited via iPhysicsTransformable
	void SetVisible(bool activeSelf) override;

	bool GetVisible() override;

};

//class PhysicsSphere : public PhysicsObject
//{
//
//};