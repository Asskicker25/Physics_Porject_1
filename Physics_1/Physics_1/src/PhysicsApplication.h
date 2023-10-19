#pragma once

#include <Graphics/ApplicationWindow.h>

class PhysicsApplication : public ApplicationWindow
{

private:
	Model sphere;
	Model dirLightModel;

	Light dirLight;

public:
	void SetUp() override;
	void PreRender() override;
	void PostRender() override;
	void ProcessInput(GLFWwindow* window) override;
	void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) override;
	void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) override;
};

