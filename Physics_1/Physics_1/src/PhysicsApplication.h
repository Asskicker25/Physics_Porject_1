#pragma once

#include <Graphics/ApplicationWindow.h>
#include <PhysicsEngine.h>

class PhysicsApplication : public ApplicationWindow
{

private:

	static const int NUM_OF_DEBUG_SPHERES = 100;

	PhysicsEngine physicsEngine;

	Model dirLightModel;

	Model sphere;
	Model sphere2;
	Model defSphere;

	Model plane;
	Model cube;

	Model poolTable;

	Model debugSpheres[NUM_OF_DEBUG_SPHERES];

	PhysicsObject spherePhyObject;
	PhysicsObject sphere2PhyObject;
	PhysicsObject planePhyObject;

	Light dirLight;


public:
	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

};

