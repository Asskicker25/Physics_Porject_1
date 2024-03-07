#include "PhysicsApplication.h"
#include "Utilities/Random.h"
#include <Softbody/SoftBodyForVertex.h>
#include <Softbody/SoftBodyForMeshes.h>
#include <Thread/PhysicsEngineThread.h>

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
	dirLight->InitializeLight(Directional);
	dirLight->intensity = 0.7;
	dirLight->attenuation = glm::vec4(1, 0.1, 0.01, 0.02);

	/*spherePhy = new PhysicsObject();
	spherePhy->name = "Sphere";
	spherePhy->LoadModel("res/Models/DefaultSphere.fbx");
	spherePhy->InitializePhysics(SPHERE,DYNAMIC);
	spherePhy->transform.SetPosition(glm::vec3(0, 3, 0));
	spherePhy->transform.SetScale(glm::vec3(0.5));
	spherePhy->transform.SetPosition(glm::vec3(0, 3, 0));

	planePhy = new PhysicsObject();
	planePhy->name = "Plane";
	planePhy->LoadModel("Assets/Models/Plane/Plane.fbx");
	planePhy->InitializePhysics(MESH_OF_TRIANGLES, STATIC);
	planePhy->transform.SetPosition(glm::vec3(0, -1.5, 0));
	planePhy->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	planePhy->transform.SetScale(glm::vec3(5.5));*/

	PhysicsEngine::GetInstance().gravity.y = -9.8f / 3.0f;
	PhysicsEngine::GetInstance().fixedStepTime = 0.01f;

	EditorLayout::GetInstance().SetMaximizeState(false);

	/*spherePhy = new PhysicsObject();
	spherePhy->name = "Sphere";
	spherePhy->LoadModel("res/Models/DefaultSphere.fbx");
	spherePhy->InitializePhysics(SPHERE, STATIC);
	spherePhy->transform.SetPosition(glm::vec3(10, 5, 0));
	spherePhy->transform.SetScale(glm::vec3(2));
	spherePhy->transform.SetPosition(glm::vec3(0, -5, -5));*/

	planePhy = new PhysicsObject();
	planePhy->name = "Plane";
	//planePhy->LoadModel("Assets/Models/Plane/Plane.fbx");
	planePhy->LoadModel("res/Models/DefaultCube.fbx");
	planePhy->InitializePhysics(AABB, STATIC);
	planePhy->transform.SetPosition(glm::vec3(0, -12.5, 0));
	planePhy->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	planePhy->transform.SetScale(glm::vec3(20,20,2));

	
	//softbody = new SoftBodyForVertex();
	//softbody->LoadModel("Assets/Models/Plane/Plane.ply");
	////softbody->LoadModel("Assets/Models/Wheel_15.ply");
	////softbody->LoadModel("Assets/Models/Plane/Flat_Grid_100x100.ply");
	////softbody->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	////softbody->transform.SetScale(glm::vec3(0.01f));
	//softbody->transform.SetScale(glm::vec3(60.0f));
	////softbody->transform.SetScale(glm::vec3(5));
	////softbody->mGravity = glm::vec3(0, -1, 0);
	//softbody->mTightness = 1.00f;
	////softbody->showDebugModels = false;
	//softbody->mNumOfIterations = 10;
	//softbody->mBounceFactor = 0.5f;
	//softbody->mNodeMaxVelocity = glm::vec3(1);
	//softbody->mGravity.y = -1.0f;

	////softbody->transform.SetPosition(glm::vec3(1.0f, 4.0f, 0));
	////softbody->AddLockNode(glm::vec3(1,2.5,1), 0.5f);
	////softbody->AddLockNode(glm::vec3(6,0,0), 2);

	//softbody->InitializeSoftBody();
	//softbody->clampVelocity = false;
	//softbody->AddCollidersToCheck(spherePhy);
	
	

	//softbody = new SoftBodyForVertex();
	//softbody->LoadModel("Assets/Models/Plane/Grid_50x50.ply");
	//softbody->transform.SetScale(glm::vec3(60.0f));
	//softbody->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	//softbody->transform.SetPosition(glm::vec3(14.0f, 0, 13));
	//softbody->AddLockNode(glm::vec3(-6, 0, 0), 2);
	////softbody->mGravity = glm::vec3(0, -1, 0);
	//softbody->InitializeSoftBody();

	/*softbody = new SoftBodyForVertex();
	softbody->LoadModel("Assets/Models/Plane/Flat_Grid_100x100.ply");
	softbody->transform.SetScale(glm::vec3(0.01f));
	softbody->transform.SetPosition(glm::vec3(14.0f, 0, 13));
	softbody->AddLockNode(glm::vec3(-6, 0, 0), 2);
	softbody->mGravity = glm::vec3(0, -1, 0);
	softbody->showDebugModels = false;
	softbody->InitializeSoftBody(); */




	softbodyMesh = new SoftBodyForMeshes();
	softbodyMesh->LoadModel("Assets/Models/Chain.fbx");
	//softbodyMesh->transform.SetRotation(glm::vec3(0, 0, 90));
	softbodyMesh->transform.SetScale(glm::vec3(1.0f));
	//softbodyMesh->mGravity = glm::vec3(0, -1, 0);
	softbodyMesh->showDebugModels = false;
	softbodyMesh->mNodeRadius = 1.5;
	//softbodyMesh->InitializeLockNodes({0});
	softbodyMesh->InitializeSoftBody();
	softbodyMesh->LockNodeAtIndex(0);
	softbodyMesh->AddCollidersToCheck(planePhy);
	softbodyMesh->mBounceFactor = 1.0f;

	
	//SoftBody* softbody2 = new SoftBody();
	//softbody2->isActive = false;
	//softbody2->LoadModel("Assets/Models/Plane/PlaneWithTex.fbx");
	//softbody2->transform.SetRotation(glm::vec3(-90.0f, 0, 0));
	//softbody2->transform.SetPosition(glm::vec3(0.0f, -4.0f, 0));
	//softbody2->mGravity = glm::vec3(0, -1, 0);
	//softbody2->showDebugModels = false;

	//softbody2->AddLockNode(glm::vec3(0), 4.0f);

	//softbody2->InitializeSoftBody();

	physicsThread->isRunning = true;
}

void PhysicsApplication::Update()
{
	PhysicsEngine::GetInstance().Update(Timer::GetInstance().deltaTime);

	/*CRITICAL_SECTION empty;
	softbody->UpdateSoftBody(Timer::GetInstance().deltaTime, empty);
	softbody->UpdateBufferData();*/


	//softbody->UpdateSoftBody(Timer::GetInstance().deltaTime, nullptr);
	//softbody->UpdateModelData(Timer::GetInstance().deltaTime);
	/*float fps = 1.0f / Timer::GetInstance().deltaTime;
	std::cout << "FPS : " << fps << std::endl;*/

	//Debugger::Print("SpherePos : ", spherePhy->transform.position);

}

void PhysicsApplication::Render()
{
	//Renderer::GetInstance().DrawAABB(GetGraphicsAabb(spherePhy->GetModelAABB()), glm::vec4(0, 1, 0, 1), false);
	
	/*Sphere* sphere = (Sphere*)spherePhy->GetTransformedPhysicsShape();

	Renderer::GetInstance().DrawSphere(sphere->position, sphere->radius, glm::vec4(0, 1, 0, 1));
	Renderer::GetInstance().DrawAABB(GetGraphicsAabb(planePhy->GetModelAABB()), glm::vec4(1, 0, 0, 1), false);*/

	//DrawAABBRecursive(planePhy->hierarchialAABB->rootNode);
	//DrawCollisionAabb(spherePhy);
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


