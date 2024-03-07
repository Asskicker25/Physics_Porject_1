#pragma once

#include <Graphics/Mesh/Model.h>
#include <functional>

#include "PhysicsShapeAndCollision.h"
#include "iPhysicsTransformable.h"
#include "PhysicsProperties.h"
#include "HierarchicalAABB.h"

#define NOMINMAX


class PhysicsObject : public Model, public iPhysicsTransformable
{
private:

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

	int modeInt = 0;
	int shapeInt = 0;
	int collisionModeInt = 0;

	const char* modeStrings[2] = { "Static", "Dynamic" };
	const char* shapeStrings[6] = { "Sphere", "Plane",  "Triangle", "AABB", "Capsule", "Mesh"};
	const char* collModeStrings[6] = { "Solid", "Trigger"};

	glm::vec4 shapeColor = glm::vec4(0, 1, 0, 1);

	void DrawPhysicsShape();
	void DrawPhysicsProperties();

public:

	bool initialized = false;
	bool isPhysicsEnabled = true;
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

	void InitializePhysics(PhysicsShape shape, PhysicsMode mode = STATIC,
		CollisionMode collisionMode = SOLID, bool isCollisionInvoke = false);

	void AssignCollisionCallback(const std::function<void(PhysicsObject*)>& collisionCallback);
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

	virtual void OnPropertyDraw();
	virtual void Render();

};

