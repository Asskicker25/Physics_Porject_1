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


public:

	Model* sphere;
	Model* plane;
	Model* hat;
	Model* pumpkin;
	Model* table;

	PhysicsObject* spherePhy;
	PhysicsObject* planePhy;
	PhysicsObject* hatPhy;
	PhysicsObject* pumpkinPhy;
	PhysicsObject* tablePhy;

	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

	void DrawAABBRecursive(HierarchicalAABBNode* node);

};

