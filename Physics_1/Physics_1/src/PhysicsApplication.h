#pragma once

#include <Graphics/ApplicationWindow.h>
#include <PhysicsEngine.h>

struct ModelAndPhysics
{
	Model* model;
	bool isPhyObj = false;
	PhysicsMode mode;
	PhysicsShape shape;
	CollisionMode collisionMode;
	glm::vec3 velocity;
	float mass;
};

class PhysicsApplication : public ApplicationWindow
{

private:
	PhysicsEngine physicsEngine;

	glm::vec4 aabbColor[3] = { 
		glm::vec4(1.0, 0.0, 0.0f, 1.0f),
		glm::vec4(0.0, 1.0, 0.0f, 1.0f),
		glm::vec4(0.0, 0.0, 1.0f, 1.0f) 
	};

	int aabbDrawDepthIndex = 0;

	int numberOfSpheres = 0;

	float xRange = 30;

public:

	Model* sphere;
	Model* plane;
	Model* plane2;
	Model* hat;
	Model* pumpkin;
	Model* table;
	Model* terrain;


	PhysicsObject* spherePhy;
	PhysicsObject* planePhy;
	PhysicsObject* planePhy2;
	PhysicsObject* hatPhy;
	PhysicsObject* pumpkinPhy;
	PhysicsObject* tablePhy;
	PhysicsObject* terrainPhy;

	std::vector<PhysicsObject*> listOfPhyObjects;


	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

	void DrawAABBRecursive(HierarchicalAABBNode* node);

};

