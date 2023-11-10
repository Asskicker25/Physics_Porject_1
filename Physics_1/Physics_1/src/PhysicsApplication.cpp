#include "PhysicsApplication.h"
#include "Utilities/Random.h"

void PhysicsApplication::SetUp()
{

	physicsEngine.gravity.y = (-9.8f / 1.0f);
	//physicsEngine.gravity.x = (-9.8f/2.0f);

	physicsEngine.fixedStepTime = 0.01f;

	camera.InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 300.0f, 60.0f);

#pragma region Lights

	for (int i = 0; i < listOfLights.size(); i++)
	{
		listOflightModels[i]->LoadModel(lightModelPath, false, false);
		listOfLights[i]->InitializeLight(*listOflightModels[i], listOfLights[i]->lightType);

		renderer.AddModel(listOflightModels[i], &lightShader);
		lightManager.AddLight(*listOfLights[i]);
	}


	lightManager.AddShader(defShader);
#pragma endregion

#pragma region Models

	for (int i = 0; i < listOfModels.size(); i++)
	{
		listOfModels[i]->model->LoadModel(modelPaths[i]);
		renderer.AddModel(*listOfModels[i]->model, defShader);

		if (!listOfModels[i]->isPhyObj) continue;

		PhysicsObject* phyObj = new PhysicsObject();

		phyObj->Initialize(listOfModels[i]->model, listOfModels[i]->shape, listOfModels[i]->mode, listOfModels[i]->collisionMode);

		phyObj->velocity = listOfModels[i]->velocity;
		phyObj->properties.SetMass(listOfModels[i]->mass);

		physicsEngine.AddPhysicsObject(phyObj);

	}

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

