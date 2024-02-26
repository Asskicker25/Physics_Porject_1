#pragma once

#include <Graphics/ApplicationWindow.h>
#include <PhysicsEngine.h>


class PhysicsApplication : public ApplicationWindow
{

private:
	int aabbDrawDepthIndex = 0;

	int numberOfSpheres = 0;

	float xRange = 30;

public:


	void SetUp() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;

	void DrawAABBRecursive(HierarchicalAABBNode* node);
	void DrawCollisionAabb(PhysicsObject* phyObj);


	// Inherited via ApplicationWindow
	virtual void Update();
	virtual void Render();
	virtual void Shutdown();

};

