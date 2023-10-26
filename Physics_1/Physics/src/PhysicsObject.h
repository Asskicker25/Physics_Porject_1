#pragma once

#include <Graphics/Model.h>
#include <functional>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"
#include "PhysicsProperties.h"


class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	Aabb cachedAABB;
	Aabb aabb;
	glm::mat4 cachedMatrix;

	std::vector < std::vector < Triangle > > triangles;
	std::vector < std::vector<Sphere*> > triangleSpheres;
	std::vector <glm::vec3> collisionPoints;

	std::function<void(PhysicsObject*)> collisionCallback = nullptr;

public:

	bool isCollisionInvoke = false;

	PhysicsMode mode = PhysicsMode::STATIC;
	PhysicsShape shape = PhysicsShape::SPHERE;
	CollisionMode collisionMode = CollisionMode::SOLID;

	PhysicsProperties properties;

	glm::vec3 position = glm::vec3(0.0f); 
	//glm::vec3 oldPosition = glm::vec3(0.0f);

	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	iShape* physicsShape;
	iShape* transformedPhysicsShape;
	void* userData;

	PhysicsObject();
	~PhysicsObject();
	
	//void Initialize(Model* model,PhysicsShape shape, PhysicsMode mode = STATIC);
	void Initialize(Model* model, PhysicsShape shape, PhysicsMode mode = STATIC,
		CollisionMode collisionMode = SOLID, bool isCollisionInvoke = false);

	void AssignCollisionCallback(const std::function<void(PhysicsObject*)>& collisionCallback);
	Model* GetModel();
	const std::function<void(PhysicsObject*)>& GetCollisionCallback();

	Aabb CalculateModelAABB();
	Aabb GetModelAABB();

	void CalculatePhysicsShape();
	iShape* GetTransformedPhysicsShape();

	void CalculateTriangleSpheres();

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

