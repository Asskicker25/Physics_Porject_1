#include "PhysicsApplication.h"
#include "Utilities/Random.h"

PhysicsObject* spherePhy;
PhysicsObject* planePhy;


void PhysicsApplication::SetUp()
{
	viewportCamera->InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 100.0f, 45.0f);
	viewportCamera->transform.SetPosition(glm::vec3(0, 0, 10));

	Renderer::GetInstance().renderMode = SHADED;

	imGuiPanelEnable = true;

	GameCamera* mainCamera = new GameCamera();
	mainCamera->name = "MainCamera";
	mainCamera->InitializeCamera(PERSPECTIVE, gameResolutionWidth, gameResolutionHeight, 0.1f, 100.0f, 45.0f);
	mainCamera->transform.SetPosition(glm::vec3(0, 0, 10));

	Light* dirLight = new Light();
	dirLight->transform.SetScale(glm::vec3(0.1f));
	dirLight->transform.SetPosition(glm::vec3(0, 0, 3));
	dirLight->InitializeLight(Directional);

	spherePhy = new PhysicsObject();
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
	planePhy->transform.SetScale(glm::vec3(5.5));

	PhysicsEngine::GetInstance().gravity.y = -9.8f / 3.0f;
	PhysicsEngine::GetInstance().fixedStepTime = 0.01f;

}

void PhysicsApplication::Update()
{
	PhysicsEngine::GetInstance().Update(Timer::GetInstance().deltaTime);
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


void PhysicsApplication::ProcessInput(GLFWwindow* window)
{
}

void PhysicsApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
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


