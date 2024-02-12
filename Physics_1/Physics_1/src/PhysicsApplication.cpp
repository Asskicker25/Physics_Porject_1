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

	Model* sphere = new Model("res/Models/DefaultSphere.fbx");
	sphere->name = "Sphere";
	sphere->transform.SetPosition(glm::vec3(0, 3, 0));
	sphere->transform.SetScale(glm::vec3(0.5));

	Model* plane = new Model("Assets/Models/Plane/Plane.fbx");
	plane->name = "Plane";
	plane->transform.SetPosition(glm::vec3(0, -1.5, 0));
	plane->transform.SetRotation(glm::vec3(-90.0f,0,0));
	plane->transform.SetScale(glm::vec3(5.5));

	spherePhy = new PhysicsObject();
	spherePhy->Initialize(sphere, SPHERE,DYNAMIC);
	physicsEngine.AddPhysicsObject(spherePhy);

	planePhy = new PhysicsObject();
	planePhy->Initialize(plane, AABB, STATIC);
	physicsEngine.AddPhysicsObject(planePhy);

	physicsEngine.gravity.y = -9.8f / 3.0f;
	physicsEngine.fixedStepTime = 0.01f;

}

void PhysicsApplication::Update()
{
	physicsEngine.Update(Timer::GetInstance().deltaTime);
}

void PhysicsApplication::Render()
{
	//Renderer::GetInstance().DrawAABB(GetGraphicsAabb(spherePhy->GetModelAABB()), glm::vec4(0, 1, 0, 1), false);
	
	Sphere* sphere = (Sphere*)spherePhy->GetTransformedPhysicsShape();

	Renderer::GetInstance().DrawSphere(sphere->position, sphere->radius, glm::vec4(0, 1, 0, 1));
	Renderer::GetInstance().DrawAABB(GetGraphicsAabb(planePhy->GetModelAABB()), glm::vec4(1, 0, 0, 1), false);
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


