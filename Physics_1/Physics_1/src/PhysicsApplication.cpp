#include "PhysicsApplication.h"
#include "Utilities/Random.h"

void PhysicsApplication::SetUp()
{

	//physicsEngine.gravity.y = (-9.8f / 1.0f);
	//physicsEngine.gravity.x = (-9.8f/2.0f);
	renderer.showNormals = true;
	renderer.SetNormalsLineScale(glm::vec3(0.01, 3.0, 0.01));

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
	sphere->transform.SetPosition(glm::vec3(3.0, 30, 0));
	sphere->transform.SetScale(glm::vec3(1));
	spherePhy = new PhysicsObject();
	spherePhy->maxDepth = 10;
	spherePhy->Initialize(sphere, SPHERE, DYNAMIC);
	physicsEngine.AddPhysicsObject(spherePhy);
	renderer.AddModel(sphere, &defShader);
	
	/*Model* sphere2 = new Model();
	sphere2->CopyFromModel(*sphere);
	sphere2->transform.SetPosition(glm::vec3(3.0, 0, 0));
	PhysicsObject* spherePhy2 = new PhysicsObject();
	spherePhy2->Initialize(sphere2, SPHERE, DYNAMIC);
	physicsEngine.AddPhysicsObject(spherePhy2);
	renderer.AddModel(sphere2, &defShader);
	spherePhy2->velocity.y = 10;*/

	//for (int i = 0; i < numberOfSpheres; i++)
	//{
	//	float xValue = GetRandomFloatNumber(-xRange, xRange);
	//	float zValue = GetRandomFloatNumber(-xRange, xRange);

	//	Model* newModel = new Model();
	//	newModel->CopyFromModel(*sphere);
	//	newModel->transform.position.x = xValue;
	//	newModel->transform.position.z = zValue;
	//	renderer.AddModel(newModel, &defShader);

	//	PhysicsObject* newPhyObj = new PhysicsObject();
	//	newPhyObj->Initialize(newModel, MESH_OF_TRIANGLES, DYNAMIC);
	//	physicsEngine.AddPhysicsObject(newPhyObj);
	//	
	//}

	//plane = new Model("Assets/Models/Plane/PlaneWithTex.fbx");
	//plane->transform.SetPosition(glm::vec3(0, -5, 0));
	//plane->transform.SetRotation(glm::vec3(-90, 0, 0));
	//plane->isWireframe = true;
	//planePhy = new PhysicsObject();
	//planePhy->maxDepth = 10;
	//planePhy->Initialize(plane, MESH_OF_TRIANGLES, STATIC);
	//physicsEngine.AddPhysicsObject(planePhy);
	//renderer.AddModel(plane, &defShader);

	hogwarts = new Model("Assets/Models/Hogwarts_3ds_export (rotated and re-normalized).ply");
	hogwarts->transform.SetPosition(glm::vec3(0, -5, 0));
	hogwarts->transform.SetRotation(glm::vec3(0, 0, 0));
	hogwarts->transform.SetScale(glm::vec3(0.001f, 0.001f, 0.001f));
	hogwarts->isWireframe = false;
	hogwartsPhy = new PhysicsObject();
	hogwartsPhy->Initialize(hogwarts, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(hogwartsPhy);
	renderer.AddModel(hogwarts, &defShader);

	renderer.selectedModel = plane2;


	/*

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
	renderer.AddModel(table, &defShader);*/
	

	/*terrain = new Model("Assets/Models/Terrain.ply");
	terrain->transform.SetPosition(glm::vec3(0, -40, 0));
	terrain->transform.SetRotation(glm::vec3(0, 0, 0));
	renderer.AddModel(terrain, &defShader);

	terrainPhy = new PhysicsObject();
	terrainPhy->Initialize(terrain, MESH_OF_TRIANGLES, STATIC);
	physicsEngine.AddPhysicsObject(terrainPhy);
	listOfPhyObjects.push_back(terrainPhy);*/


}

void PhysicsApplication::PreRender()
{

}

void PhysicsApplication::PostRender()
{
	physicsEngine.Update(deltaTime);

	/*Sphere* sphere1 = dynamic_cast<Sphere*>(planePhy->GetTransformedPhysicsShape());
	Sphere* sphere2 = dynamic_cast<Sphere*>(planePhy2->GetTransformedPhysicsShape());

	renderer.DrawSphere(sphere1->position, sphere1->radius);
	renderer.DrawSphere(sphere2->position, sphere2->radius);*/

	/*renderer.DrawAABB(GetGraphicsAabb(planePhy2->GetModelAABB()), aabbColor[2]);
	renderer.DrawAABB(GetGraphicsAabb(planePhy->GetModelAABB()), aabbColor[2]);*/

	/*DrawAABBRecursive(spherePhy->hierarchialAABB->rootNode);
	DrawAABBRecursive(planePhy->hierarchialAABB->rootNode);*/
	//DrawAABBRecursive(testphy->hierarchialAABB->rootNode);


	//DrawAABBRecursive(planePhy->hierarchialAABB->rootNode);
	/*DrawAABBRecursive(hatPhy->hierarchialAABB->rootNode);
	DrawAABBRecursive(pumpkinPhy->hierarchialAABB->rootNode);
	DrawAABBRecursive(tablePhy->hierarchialAABB->rootNode);*/
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
			hat->transform.SetRotation(glm::vec3(
				hat->transform.rotation.x + 30, hat->transform.rotation.y, hat->transform.rotation.z));
			hat->transform.scale.x += 5;
			hat->transform.scale.y += 5;
			hat->transform.scale.z += 5;

		}
		else if (key == GLFW_KEY_DOWN)
		{
			hat->transform.position.x -= 5;
			hat->transform.SetRotation(glm::vec3(
				hat->transform.rotation.x - 30, hat->transform.rotation.y, hat->transform.rotation.z));
			hat->transform.scale.x -= 5;
			hat->transform.scale.y -= 5;
			hat->transform.scale.z -= 5;

		}
		else if (key == GLFW_KEY_SPACE)
		{
			for (PhysicsObject* phyObj : listOfPhyObjects)
			{
				phyObj->useBvh = !phyObj->useBvh;
			}
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
		renderer.DrawAABB(GetGraphicsAabb(node->GetModelAABB()), aabbColor[2]);
		return;
	}*/

	if (node->triangleIndices.size() != 0)
	{
		renderer.DrawAABB(GetGraphicsAabb(node->GetModelAABB()), aabbColor[2]);
	}

	if (node->leftNode == nullptr) return;

	DrawAABBRecursive(node->leftNode);
	DrawAABBRecursive(node->rightNode);

}

