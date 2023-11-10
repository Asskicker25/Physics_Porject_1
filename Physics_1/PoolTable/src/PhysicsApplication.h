#pragma once

#include <Graphics/ApplicationWindow.h>
#include <PhysicsEngine.h>

class PhysicsApplication : public ApplicationWindow
{

private:

	static const int NUM_OF_DEBUG_SPHERES = 100;
	static const int NUM_OF_BALLS = 20;

	PhysicsEngine physicsEngine;

	Model dirLightModel;

	Model poolTable;
	Model debugCube;
	//Model defCube;

	PhysicsObject poolTablePhy;

	Light dirLight;

	Model* currentModel;
	Light* currentLight;

	std::vector<PhysicsObject*> poolBalls;


	bool moveObject = false;


public:
	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

	const float& GetRandomNumber(float minValue, float maxValue);
};

