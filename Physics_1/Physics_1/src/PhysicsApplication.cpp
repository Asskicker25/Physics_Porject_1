#include "PhysicsApplication.h"
#include "Utilities/Random.h"
#include <Softbody/SoftBodyForVertex.h>
#include <Softbody/SoftBodyForMeshes.h>
#include <Thread/PhysicsEngineThread.h>

#include "Player/Snake.h"

using namespace Verlet;

PhysicsObject* spherePhy;
PhysicsObject* planePhy;

SoftBodyForVertex* softbody;
SoftBodyForMeshes* softbodyMesh;

PhysicsEngineThreadInfo* physicsThread = nullptr;

void PhysicsApplication::SetUp()
{
	physicsThread = InitializePhysicsThread(0.01f);

	viewportCamera->InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 100.0f, 45.0f);
	viewportCamera->transform.SetPosition(glm::vec3(0, 0, 30));

	Renderer::GetInstance().renderMode = SHADED;

	imGuiPanelEnable = true;

	GameCamera* mainCamera = new GameCamera();
	mainCamera->name = "MainCamera";
	mainCamera->InitializeCamera(PERSPECTIVE, gameResolutionWidth, gameResolutionHeight, 0.1f, 100.0f, 45.0f);
	mainCamera->transform.SetPosition(glm::vec3(0, 0, 10));

	Light* dirLight = new Light();
	dirLight->transform.SetScale(glm::vec3(0.1f));
	dirLight->transform.SetPosition(glm::vec3(0, 5, 3));
	dirLight->transform.SetRotation(glm::vec3(60, 0, 180));
	dirLight->InitializeLight(Directional);
	dirLight->intensity = 0.7;
	dirLight->attenuation = glm::vec4(1, 0.1, 0.01, 0.02);

	PhysicsEngine::GetInstance().gravity.y = -9.8f / 3.0f;
	PhysicsEngine::GetInstance().fixedStepTime = 0.01f;

	EditorLayout::GetInstance().SetMaximizeState(false);


	//std::vector<PhysicsObject*> listOfPhyObject;

	planePhy = new PhysicsObject();
	planePhy->name = "Plane";
	//planePhy->LoadModel("Assets/Models/Plane/Plane.fbx");
	planePhy->LoadModel("res/Models/DefaultCube.fbx");
	planePhy->InitializePhysics(AABB, STATIC);
	planePhy->transform.SetPosition(glm::vec3(0, -12.5, 0));
	planePhy->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	planePhy->transform.SetScale(glm::vec3(20,20,2));


	//listOfPhyObject.push_back(planePhy);

	softbodyMesh = new SoftBodyForMeshes();
	softbodyMesh->LoadModel("Assets/Models/Rope_2_Normals.fbx");
	softbodyMesh->transform.SetRotation(glm::vec3(0, 0, 0));
	softbodyMesh->transform.SetScale(glm::vec3(0.5f));
	softbodyMesh->mGravity = glm::vec3(0, -1, 0);
	//softbodyMesh->showDebugModels = false;
	softbodyMesh->mNodeRadius = 0.5f;
	softbodyMesh->InitializeLockNodes({ 0 });
	softbodyMesh->InitializeSoftBody();
	softbodyMesh->LockNodeAtIndex(0);
	softbodyMesh->AddCollidersToCheck(planePhy);
	softbodyMesh->mBounceFactor = 1.0f;

	//Snake* snake = new Snake(listOfPhyObject);


	physicsThread->isRunning = true;
}

void PhysicsApplication::Update()
{
	PhysicsEngine::GetInstance().Update(Timer::GetInstance().deltaTime);



}

void PhysicsApplication::Render()
{

	
}

void PhysicsApplication::Shutdown()
{
	if (physicsThread != nullptr)
	{
		physicsThread->isAlive = false;
		WaitForSingleObject(physicsThread->threadHandle, INFINITE);
		CloseHandle(physicsThread->threadHandle);
		DeleteCriticalSection(&physicsThread->softBodyUpdateModelData);
	}
	PhysicsEngine::GetInstance().Shutdown();
}

void PhysicsApplication::OnPlayStateChanged(bool state)
{
}


void PhysicsApplication::ProcessInput(GLFWwindow* window)
{
}

void PhysicsApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_SPACE)
		{
			softbody->DisconnectRandomNode();
			//softbodyMesh->AddForceToRandomNode(glm::vec3(0, 50, 0));
		}
		else if (key == GLFW_KEY_ENTER)
		{
			softbodyMesh->mGravity = glm::vec3(0, -1, 0);

			//delete softbody->mListOfNod
			// es[0];
			//softbody->mListOfNodes.erase(softbody->mListOfNodes.begin());
			//softbody->mGravity = glm::vec3(0, -1, 0);
		}
	}

}

void PhysicsApplication::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}

void PhysicsApplication::DrawAABBRecursive(HierarchicalAABBNode* node)
{
	if (node == nullptr) return;

	if (node->triangleIndices.size() != 0)
	{
		Renderer::GetInstance().DrawAABB(GetGraphicsAabb(node->GetModelAABB()), glm::vec4(1));
	}

	if (node->leftNode == nullptr) return;

	DrawAABBRecursive(node->leftNode);
	DrawAABBRecursive(node->rightNode);

}

void PhysicsApplication::DrawCollisionAabb(PhysicsObject* phyObj)
{
	std::vector<Aabb> collisionAabs = phyObj->GetCollisionAabbs();

	for (Aabb aabb : collisionAabs)
	{
		Renderer::GetInstance().DrawAABB(GetGraphicsAabb(aabb), glm::vec4(1));
	}
}


