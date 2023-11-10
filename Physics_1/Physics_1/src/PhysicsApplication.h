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

	static const int NUM_OF_DEBUG_SPHERES = 100;

	const std::string& lightModelPath = "Assets/Models/DefaultSphere.fbx";


	PhysicsEngine physicsEngine;

	Model debugSpheres[NUM_OF_DEBUG_SPHERES];


public:

	std::vector<ModelAndPhysics*> listOfModels;
	std::vector<Model*> listOflightModels;
	std::vector<std::string> modelPaths;
	std::vector<Light*> listOfLights;


	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

};

