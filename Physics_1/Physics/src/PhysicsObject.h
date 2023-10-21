#pragma once

#include <Graphics/Model.h>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"

class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	float mass = 1.0f;

	Aabb cachedAABB;
	Aabb aabb;
	glm::mat4 cachedMatrix;

	std::vector < std::vector < Triangle > > triangles;
	std::vector < std::vector<Sphere*> > triangleSpheres;
	std::vector <glm::vec3> collisionPoints;

public:
	PhysicsMode mode = PhysicsMode::STATIC;
	PhysicsShape shape = PhysicsShape::SPHERE;

	//Since division is slow
	float inverse_mass = 1.0f;

	glm::vec3 position = glm::vec3(0.0f); 
	glm::vec3 oldPosition = glm::vec3(0.0f);

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

	void SetMass(float mass);
	float GetMass();

	bool CheckCollision(PhysicsObject* other,
		std::vector<glm::vec3>& collisionPoints,
		std::vector<glm::vec3>& collisionNormals);
	
	const std::vector < std::vector < Triangle > >& GetTriangleList();
	const std::vector < std::vector<Sphere*> >& GetSphereList();
	const std::vector <glm::vec3>& GetCollisionPoints();
	void SetCollisionPoints(const std::vector <glm::vec3>& collisionPoints);

	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override;
	void SetVisible(bool activeSelf) override;

	bool GetVisible() override;

};

