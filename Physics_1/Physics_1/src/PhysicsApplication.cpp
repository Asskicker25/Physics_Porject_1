#include "PhysicsApplication.h"

void PhysicsApplication::SetUp()
{
	//physicsEngine.fixedStepTime = 1.0f;
#pragma region Light

	dirLightModel.LoadModel("Assets/Models/DefaultSphere.fbx", false, false);
	dirLightModel.transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	dirLightModel.transform.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	dirLightModel.isActive = false;

	dirLight.InitializeLight(dirLightModel, LightType::Directional);
	dirLight.SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

#pragma endregion

#pragma region Model

	sphere.LoadModel("Assets/Models/SpecSphere/Sphere 1.fbx");
	sphere.modelId = "Sphere";
	sphere.transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	sphere.transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sphere.isWireframe = true;

	plane.LoadModel("Assets/Models/Plane/PlaneWithTex.fbx");
	plane.modelId = "Plane";
	plane.transform.SetScale(glm::vec3(5.0f));
	plane.transform.SetPosition(glm::vec3(1.0f, -5.0f, 0.0f));
	plane.transform.SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
	//plane.isWireframe = true;

#pragma endregion

#pragma region Physics

	physicsEngine.fixedStepTime = 0.01f;

	spherePhyObject.Initialize(&sphere);
	spherePhyObject.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	spherePhyObject.acceleration.y = (-9.8f / 5.0f);

	planePhyObject.Initialize(&plane);

	physicsEngine.AddPhysicsObject(&spherePhyObject);
	physicsEngine.AddPhysicsObject(&planePhyObject);

#pragma endregion

#pragma region Renderers

	renderer.AddModel(dirLightModel, lightShader);
	renderer.AddModel(sphere, defShader);
	renderer.AddModel(plane, defShader);

	lightManager.AddLight(dirLight);
	lightManager.AddShader(defShader);

#pragma endregion

}

void PhysicsApplication::PreRender()
{
}

void PhysicsApplication::PostRender()
{
	physicsEngine.Update(deltaTime);
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
