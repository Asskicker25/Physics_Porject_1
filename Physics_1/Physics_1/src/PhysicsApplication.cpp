#include "PhysicsApplication.h"
#include "Utilities/Random.h"

void PhysicsApplication::SetUp()
{

	//physicsEngine.gravity.y = (-9.8f / 1.0f);
	//physicsEngine.gravity.x = (-9.8f/2.0f);

	physicsEngine.fixedStepTime = 0.01f;
	renderer.wireframeMaterial->SetBaseColor(glm::vec4(0));

	camera->cameraPos.z = 10;
	camera->InitializeCamera(PERSPECTIVE, windowWidth, windowHeight, 0.1f, 300.0f, 60.0f);

	Model* lightModel = new Model("res/Models/DefaultSphere.fbx");
	lightModel->transform.SetScale(glm::vec3(0.1));

	Light* dirLight = new Light();
	dirLight->InitializeLight(lightModel, Directional);

	lightManager.AddLight(dirLight);

	sphere = new Model("Assets/Models/DefaultSphere.fbx");
	sphere->transform.SetPosition(glm::vec3(3.0, 0, 0));
	sphere->transform.SetScale(glm::vec3(1));
	spherePhy = new PhysicsObject();
	spherePhy->Initialize(sphere, SPHERE, DYNAMIC);
	physicsEngine.AddPhysicsObject(spherePhy);
	renderer.AddModel(sphere, &defShader);


	plane = new Model("Assets/Models/Plane/PlaneWithTex.fbx");
	plane->transform.SetPosition(glm::vec3(0, -5, 0));
	plane->transform.SetRotation(glm::vec3(-90, 0, 0));
	plane->isWireframe = true;
	planePhy = new PhysicsObject();
	planePhy->Initialize(plane, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(planePhy);
	renderer.AddModel(plane, &defShader);

	hat = new Model("Assets/Models/Hat.fbx");
	hat->transform.SetPosition(glm::vec3(0, 0, 0));
	hat->transform.SetRotation(glm::vec3(-90, 0, 0));
	hat->transform.SetScale(glm::vec3(50));
	hat->isWireframe = true;
	hatPhy = new PhysicsObject();
	hatPhy->Initialize(hat, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(hatPhy);
	renderer.AddModel(hat, &defShader);


	pumpkin = new Model("Assets/Models/Pumpkin.fbx");
	pumpkin->transform.SetPosition(glm::vec3(-30, 0, 0));
	pumpkin->transform.SetRotation(glm::vec3(-90, 0, 0));
	pumpkin->transform.SetScale(glm::vec3(50));
	pumpkin->isWireframe = true;
	pumpkinPhy = new PhysicsObject();
	pumpkinPhy->Initialize(pumpkin, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(pumpkinPhy);
	renderer.AddModel(pumpkin, &defShader);

	table = new Model("Assets/Models/Table.fbx");
	table->transform.SetPosition(glm::vec3(60, 0, 0));
	table->transform.SetRotation(glm::vec3(-90, 0, 0));
	table->transform.SetScale(glm::vec3(50));
	table->isWireframe = true;
	tablePhy = new PhysicsObject();
	tablePhy->Initialize(table, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(tablePhy);
	renderer.AddModel(table, &defShader);
}

void PhysicsApplication::PreRender()
{

}

void PhysicsApplication::PostRender()
{
	physicsEngine.Update(deltaTime);

	DrawAABBRecursive(hatPhy->hierarchialAABB->rootNode);
	DrawAABBRecursive(pumpkinPhy->hierarchialAABB->rootNode);
	DrawAABBRecursive(tablePhy->hierarchialAABB->rootNode);
	//renderer.DrawAABB(GetGraphicsAabb(planePhy->GetModelAABB()), abbColor);
}

void PhysicsApplication::ProcessInput(GLFWwindow* window)
{
}

void PhysicsApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT)
		{
			aabbDrawDepthIndex--;
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			aabbDrawDepthIndex++;
		}
		else if (key == GLFW_KEY_UP)
		{
			hat->transform.position.x += 5;
		}
		else if (key == GLFW_KEY_DOWN)
		{
			hat->transform.position.x -= 5;
		}
	}
}

void PhysicsApplication::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}

void PhysicsApplication::DrawAABBRecursive(HierarchicalAABBNode* node)
{
	if (node == nullptr) return;

	/*if (node->nodeIndex == aabbDrawDepthIndex)
	{
		renderer.DrawAABB(GetGraphicsAabb(node->aabb), aabbColor[2]);
		return;
	}*/

	if (node->triangleIndices.size() != 0)
	{
		renderer.DrawAABB(GetGraphicsAabb(node->GetAABB()), aabbColor[2]);
	}

	if (node->leftNode == nullptr) return;

	DrawAABBRecursive(node->leftNode);
	DrawAABBRecursive(node->rightNode);

}

