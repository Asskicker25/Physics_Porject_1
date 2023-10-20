#pragma once

#include <Graphics/ApplicationWindow.h>
#include <PhysicsEngine.h>

class PhysicsApplication : public ApplicationWindow
{

private:
	PhysicsEngine physicsEngine;

	Model dirLightModel;

	Model sphere;
	Model sphere2;

	Model plane;
	Model cube;

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

