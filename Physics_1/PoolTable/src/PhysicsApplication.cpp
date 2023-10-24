#include "PhysicsApplication.h"
#include <random>

void PhysicsApplication::SetUp()
{
	physicsEngine.gravity.y = (-9.8f/2.0f);

	physicsEngine.fixedStepTime = 0.02f;

	cameraPos.y = 24.4914f;
	cameraPos.z = 21.3531f;
	cameraPitch = -40;

	moveSpeed = 5;

#pragma region Light

	dirLightModel.LoadModel("Assets/Models/DefaultSphere.fbx", false, false);
	dirLightModel.transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	dirLightModel.transform.SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	dirLightModel.isActive = false;

	dirLight.InitializeLight(dirLightModel, LightType::Directional);
	dirLight.SetLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
	dirLight.intensity = 2.0f;

#pragma endregion

#pragma region Model

	poolTable.LoadModel("Assets/Models/PoolTable/pooltable.fbx");
	poolTable.transform.SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	poolTable.transform.SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));
	poolTable.transform.SetScale(glm::vec3(0.01f));
	poolTable.isWireframe = false;

	Model* rightCube = new Model();
	rightCube->LoadModel("Assets/Models/DefaultCube.fbx");
	rightCube->transform.SetPosition(glm::vec3(12.2667f, 6.9271f, 0.709591f));
	rightCube->transform.SetScale(glm::vec3(1.0f,1.0f,10.0f));

	Model* leftCube = new Model();
	*leftCube = *rightCube;
	leftCube->transform.SetPosition(glm::vec3(-12.2667f, 6.9271f, 0.709591f));

	Model* frontCube = new Model();
	*frontCube = *rightCube;
	frontCube->transform.SetPosition(glm::vec3(0, 6.9271f, 6.0f));
	frontCube->transform.SetScale(glm::vec3(20.0f,1.0f, 1.0f));

	Model* backCube = new Model();
	*backCube = *frontCube;
	backCube->transform.SetPosition(glm::vec3(0, 6.9271f, -7.0f));

	Model* topCube = new Model();
	*topCube = *frontCube;
	topCube->transform.SetPosition(glm::vec3(0, 8.3f, -7.0f));
	topCube->transform.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));

	debugCube = *frontCube;
	debugCube.transform.SetPosition(glm::vec3(0.0f, 15.0f, 0.0));
	debugCube.transform.SetScale(glm::vec3(1.0f));

	//-1.58232, 6.43176, -1.13238
	
#pragma endregion


#pragma region PoolBalls

	Model* poolBall = new Model();
	poolBall->LoadModel("Assets/Models/PoolTable/PoolBalll.fbx");
	poolBall->transform.SetPosition(glm::vec3(-1.58232f, 6.43176f + 0.5f, -1.13238f));
	//poolBall.transform.SetPosition(glm::vec3(-1.58232f, 6.43176f + 5.0f, -1.13238f));
	poolBall->transform.SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));
	poolBall->transform.SetScale(glm::vec3(0.01f));


	for (int i = 0; i < NUM_OF_BALLS; i++)
	{
		Model* newBall = new Model();
		*newBall = *poolBall;

		newBall->transform.position.x = GetRandomNumber(-6, 6);
		newBall->transform.position.z = GetRandomNumber(-6, 6);

		PhysicsObject* ballPhy = new PhysicsObject();
		ballPhy->Initialize(newBall, SPHERE, DYNAMIC);


		renderer.AddModel(*newBall,defShader);
		physicsEngine.AddPhysicsObject(ballPhy);
		poolBalls.push_back(ballPhy);
	}

	/*Model* poolBall2 = new Model();
	*poolBall2 = *poolBall;
	poolBall2->transform.SetPosition(glm::vec3(-1.58232f + 5.0f, 6.43176f + 0.5f, -1.13238f));*/

#pragma endregion


#pragma region Physics

	poolTablePhy.Initialize(&poolTable, AABB, STATIC);

	PhysicsObject* rightCubePhy = new PhysicsObject();
	rightCubePhy->Initialize(rightCube, AABB, STATIC);

	PhysicsObject* leftCubePhy = new PhysicsObject();
	leftCubePhy->Initialize(leftCube, AABB, STATIC);

	PhysicsObject* frontCubePhy = new PhysicsObject();
	frontCubePhy->Initialize(frontCube, AABB, STATIC);

	PhysicsObject* backCubePhy = new PhysicsObject();
	backCubePhy->Initialize(backCube, AABB, STATIC);

	PhysicsObject* topCubePhy = new PhysicsObject();
	topCubePhy->Initialize(topCube, AABB, STATIC);

	PhysicsObject* poolBallPhy = new PhysicsObject();
	poolBallPhy->Initialize(poolBall, SPHERE, DYNAMIC);
	//poolBallPhy.velocity = glm::vec3(20.5f, 0.0f, 0.0f);
	poolBallPhy->velocity = glm::vec3(0.0f, 0.f, 0.0f);
	poolBallPhy->properties.SetMass(1.0f);

	/*PhysicsObject* poolBall2Phy = new PhysicsObject();
	poolBall2Phy->Initialize(poolBall2, SPHERE, DYNAMIC);
	poolBall2Phy->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	poolBall2Phy->properties.SetMass(1.0f);*/

	physicsEngine.AddPhysicsObject(&poolTablePhy);
	physicsEngine.AddPhysicsObject(rightCubePhy);
	physicsEngine.AddPhysicsObject(leftCubePhy);
	physicsEngine.AddPhysicsObject(frontCubePhy);
	physicsEngine.AddPhysicsObject(backCubePhy);
	physicsEngine.AddPhysicsObject(topCubePhy);
	physicsEngine.AddPhysicsObject(poolBallPhy);
	//physicsEngine.AddPhysicsObject(poolBall2Phy);

	poolBalls.push_back(poolBallPhy);
	//poolBalls.push_back(poolBall2Phy);

#pragma endregion

#pragma region DebugSphere


#pragma endregion

#pragma region Renderers

	renderer.AddModel(poolTable, defShader);
	/*renderer.AddModel(*rightCube, defShader);
	renderer.AddModel(*leftCube, defShader);
	renderer.AddModel(*frontCube, defShader);
	renderer.AddModel(*topCube, defShader);
	renderer.AddModel(*backCube, defShader);*/
	renderer.AddModel(*poolBall, defShader);
	//renderer.AddModel(*poolBall2, defShader);
	//renderer.AddModel(debugCube, defShader);

	lightManager.AddLight(dirLight);
	lightManager.AddShader(defShader);

#pragma endregion

	currentModel = topCube;
	currentLight = &dirLight;
}

void PhysicsApplication::PreRender()
{
	//Debugger::Print(currentModel->transform.position);
	debugCube.transform.position = poolTablePhy.GetModelAABB().min;
}

void PhysicsApplication::PostRender()
{
	physicsEngine.Update(deltaTime);
}

void PhysicsApplication::ProcessInput(GLFWwindow* window)
{
	float speed = moveSpeed * deltaTime;


	if (!moveObject) return;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		currentModel->transform.position.z -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{

		currentModel->transform.position.z += speed;

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		currentModel->transform.position.x -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		currentModel->transform.position.x += speed;
		//std::cout << currentModel.transform.position.x << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		currentModel->transform.position.y += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		currentModel->transform.position.y -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		currentLight->intensity += 0.05f * speed;
		//ambientStrength += 0.05f * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		currentLight->intensity -= 0.05f * speed;
		//ambientStrength -= 0.05f * speed;
	}


	std::stringstream ssTitle;
	ssTitle << "Light Intensity : " << currentLight->intensity
		<< "  LightPos:"
		<< currentLight->transform->position.x << " , "
		<< currentLight->transform->position.y << " , "
		<< currentLight->transform->position.z<<
		
		"Model Pos : " << currentModel->transform.position.x << " , "
		<< currentModel->transform.position.y << " , "
		<< currentModel->transform.position.z;

	std::string theTitle = ssTitle.str();

	glfwSetWindowTitle(window, theTitle.c_str());
}

void PhysicsApplication::KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_SPACE)
		{
			stopKeyCallback = !stopKeyCallback;
			moveObject = !moveObject;
		}

		if (key == GLFW_KEY_P)
		{

			for (PhysicsObject* ball : poolBalls)
			{
				ball->velocity.x = GetRandomNumber(-20.0f, 20.0f);
				ball->velocity.z = GetRandomNumber(-20.0f, 20.0f);
			}
			/*poolBallPhy.velocity.x = GetRandomNumber(-15.0f, 15.0f);
			poolBallPhy.velocity.z = GetRandomNumber(-15.0f, 15.0f);

			poolBall2Phy.velocity.x = GetRandomNumber(-15.0f, 15.0f);
			poolBall2Phy.velocity.z = GetRandomNumber(-15.0f, 15.0f);*/
		}
	}
}

void PhysicsApplication::MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
}

const float& PhysicsApplication::GetRandomNumber(float minValue, float maxValue)
{
	std::random_device rd;  // Initialize a random seed
	std::mt19937 gen(rd()); // Mersenne Twister 19937 random number generator
	std::uniform_real_distribution<float> distribution(minValue, maxValue); // Define the range

	float randomFloat = distribution(gen); // Generate a random float between minValue and maxValue

	return randomFloat;
}
