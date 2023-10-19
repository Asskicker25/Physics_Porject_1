#include "PhysicsApplication.h"

void PhysicsApplication::SetUp()
{
	dirLightModel.LoadModel("Assets/Models/DefaultSphere.fbx", false, false);
	dirLightModel.transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	dirLightModel.transform.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	dirLightModel.isActive = false;

	dirLight.InitializeLight(dirLightModel, LightType::Point);
	dirLight.SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

	sphere.LoadModel("Assets/Models/SpecSphere/Sphere 1.fbx");
	sphere.transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	sphere.transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	renderer.AddModel(dirLightModel,lightShader);
	renderer.AddModel(sphere,defShader);

	lightManager.AddLight(dirLight);
	lightManager.AddShader(defShader);
}

void PhysicsApplication::PreRender()
{
}

void PhysicsApplication::PostRender()
{
}

void PhysicsApplication::ProcessInput(GLFWwindow* window)
{
}

void PhysicsApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
}

void PhysicsApplication::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}
