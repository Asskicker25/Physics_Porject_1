#pragma once

#include <Graphics/Model.h>
#include <functional>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"
#include "PhysicsProperties.h"
#include "HierarchicalAABB.h"

class PhysicsObject : public iPhysicsTransformable
{
private:
	Model* model = nullptr;

	Aabb cachedAABB;
	Aabb aabb;
	glm::mat4 cachedMatrix;

	std::vector <Triangle> triangles;
	std::vector <Sphere*>  triangleSpheres;
	std::vector <glm::vec3> collisionPoints;
	std::vector <glm::vec3> collisionNormals;
	std::vector<Aabb> collisionAabbs;
	std::vector<PhysicsObject*> listOfExcludingPhyObjects;

	std::function<void(PhysicsObject*)> collisionCallback = nullptr;

public:

	bool isEnabled = true;
	bool isCollisionInvoke = false;
	bool useBvh = true;
	float maxDepth = 10;

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
	HierarchicalAABB* hierarchialAABB;
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
	Aabb GetAABB();

	void AddExludingPhyObj(PhysicsObject* phyObj);
	bool CheckIfExcluding(PhysicsObject* phyObj);

	void CalculatePhysicsShape();
	iShape* GetTransformedPhysicsShape();

	void CalculateTriangleSpheres();

	bool CheckCollision(PhysicsObject* other,
		std::vector<glm::vec3>& collisionPoints,
		std::vector<glm::vec3>& collisionNormals);

	const std::vector < Triangle >& GetTriangleList();
	const std::vector < Sphere* >& GetSphereList();
	const std::vector <glm::vec3>& GetCollisionPoints();
	const std::vector <glm::vec3>& GetCollisionNormals();
	const std::vector<Aabb>& GetCollisionAabbs();
	void SetCollisionPoints(const std::vector <glm::vec3>& collisionPoints);
	void SetCollisionNormals(const std::vector <glm::vec3>& collisionNormals);
	void SetCollisionAabbs(const std::vector <Aabb>& collisionAabs);

	// Inherited via iPhysicsTransformable
	glm::vec3 GetPosition() override;
	glm::vec3 GetRotation() override;
	void SetPosition(const glm::vec3& newPosition) override;
	void SetDrawOrientation(const glm::vec3& newOrientation) override;
	void SetVisible(bool activeSelf) override;

	bool GetVisible() override;

};

