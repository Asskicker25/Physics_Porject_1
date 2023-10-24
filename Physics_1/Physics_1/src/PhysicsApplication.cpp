#include "PhysicsApplication.h"

void PhysicsApplication::SetUp()
{
	physicsEngine.gravity.y = (-9.8f/2.0f);
	cameraPos.y = 5.0f;

#pragma region Light

	dirLightModel.LoadModel("Assets/Models/DefaultSphere.fbx", false, false);
	dirLightModel.transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	dirLightModel.transform.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	dirLightModel.isActive = false;

	dirLight.InitializeLight(dirLightModel, LightType::Directional);
	dirLight.SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
	dirLight.intensity = 1.5f;


#pragma endregion

#pragma region Model

	defSphere.LoadModel("Assets/Models/DefaultSphere.fbx");
	defSphere.transform.SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
	defSphere.isWireframe = false;

	sphere.LoadModel("Assets/Models/SpecSphere/Sphere 1.fbx");
	sphere.modelId = "Sphere";
	sphere.transform.SetPosition(glm::vec3(-3.0f, 10.0f, 0.0f)); 
	sphere.transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	sphere.isWireframe = true;

	sphere2 = sphere;
	sphere2.transform.SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	sphere2.transform.SetScale(glm::vec3(1.0f));
	sphere2.isWireframe = false;

	/*plane.LoadModel("Assets/Models/Plane/PlaneWithTex.fbx");
	plane.transform.SetScale(glm::vec3(3.0f));
	plane.transform.SetPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	plane.transform.SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));*/

	plane.modelId = "Terrain";
	plane.LoadModel("Assets/Models/Terrain.ply",false, false);
	plane.transform.SetPosition(glm::vec3(1.0f, -50.0f, 0.0f));
	plane.transform.SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	plane.isWireframe = true;

	/*cube.LoadModel("Assets/Models/DefaultCube.fbx");
	cube.transform.SetScale(glm::vec3(0.05f));*/

#pragma endregion

#pragma region Physics

	physicsEngine.fixedStepTime = 0.01f;

	spherePhyObject.Initialize(&sphere, SPHERE, DYNAMIC);
	spherePhyObject.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//spherePhyObject.acceleration.y = (-9.8f / 5.0f);
	spherePhyObject.properties.SetMass(0.65f);
	spherePhyObject.properties.bounciness = 0.9f;

	sphere2PhyObject.Initialize(&sphere2, SPHERE, DYNAMIC);
	sphere2PhyObject.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//sphere2PhyObject.acceleration.y = (-9.8f / 2.0f);
	sphere2PhyObject.properties.SetMass(1.75f);
	sphere2PhyObject.properties.bounciness = 0.9f;

	planePhyObject.Initialize(&plane, MESH_OF_TRIANGLES,STATIC);
	/*planePhyObject.velocity = glm::vec3(0.0f, 0.5f, 0.0f);
	planePhyObject.acceleration.y = (9.8f / 2.0f);
	planePhyObject.SetMass(10.0f);*/

	physicsEngine.AddPhysicsObject(&spherePhyObject);
	physicsEngine.AddPhysicsObject(&sphere2PhyObject);
	physicsEngine.AddPhysicsObject(&planePhyObject);

#pragma endregion

#pragma region DebugSphere

	//const std::vector<std::vector<Triangle>>& triangleList = planePhyObject.GetTriangleList();

	/*for (int i = 0; i < NUM_OF_DEBUG_SPHERES; i++)
	{
		debugSpheres[i] = defSphere;
	}
	
	physicsEngine.SetDebugSpheres(debugSpheres, NUM_OF_DEBUG_SPHERES);*/

#pragma endregion

#pragma region Renderers

	renderer.AddModel(dirLightModel, lightShader);
	renderer.AddModel(sphere, defShader);
	renderer.AddModel(sphere2, defShader);
	renderer.AddModel(plane, defShader);
	//renderer.AddModel(cube, defShader);
	/*for (int i = 0; i < NUM_OF_DEBUG_SPHERES; i++)
	{
		renderer.AddModel(debugSpheres[i],defShader);
	}*/

	lightManager.AddLight(dirLight);
	lightManager.AddShader(defShader);

#pragma endregion

}

void PhysicsApplication::PreRender()
{
	/*Sphere* sphereBod = dynamic_cast<Sphere*> (sphere2PhyObject.GetTransformedPhysicsShape());
	glm::vec3 newPos = glm::vec3(sphereBod->position.x + sphereBod->radius, sphereBod->position.y, sphereBod->position.z);
	cube.transform.SetPosition(newPos);*/

	//cube.transform.SetPosition(sphere2PhyObject.GetModelAABB().min);
	//std::cout << planePhyObject.GetModelAABB().max.x << ",";
	//std::cout << planePhyObject.GetModelAABB().max.y << ",";
	//std::cout << planePhyObject.GetModelAABB().max.z << std::endl;

	//Debugger::Print(sphere2PhyObject.GetPosition());
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
