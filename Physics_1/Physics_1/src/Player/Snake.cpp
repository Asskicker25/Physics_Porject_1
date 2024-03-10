#include "Snake.h"
#include <Graphics/Camera/CameraSystem.h>

Snake::Snake(std::vector<PhysicsObject*>& listOfPhysicsObject)
{
	LoadModel("Assets/Models/Chain.fbx");
	transform.SetPosition(glm::vec3(0, 10, 0));
	transform.SetRotation(glm::vec3(0, 0, 0));
	transform.SetScale(glm::vec3(0.5f));
	mGravity = glm::vec3(0, -1, 0);
	//softbodyMesh->showDebugModels = false;
	mNodeRadius = 0.5f;
	InitializeLockNodes({ 0 });
	InitializeSoftBody();
	LockNodeAtIndex(0);
	mBounceFactor = 2.0f;

	for (PhysicsObject* phy : listOfPhysicsObject)
	{
		AddCollidersToCheck(phy);
	}

	mHead = mListOfNodes[0];

	mMainCamera = CameraSystem::GetInstance().GetMainCamera();

	for (MeshAndMaterial* mesh : meshes)
	{
		mesh->material->AsMaterial()->SetBaseColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	}

	InputManager::GetInstance().AddListener(this);
}

void Snake::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_Q)
	{
		mDownHeld = true;
	}
	else if (key == GLFW_KEY_E)
	{
		mUpHeld = true;
	}
}

void Snake::OnKeyReleased(const int& key)
{
	if (key == GLFW_KEY_Q)
	{
		mDownHeld = false;

	}
	else if (key == GLFW_KEY_E)
	{
		mUpHeld = false;
	}
}

void Snake::Update(float deltaTime)
{
	Model::Update(deltaTime);
	SetInput();
	HandleMove(deltaTime);

}

void Snake::HandleMove(float deltaTime)
{
	glm::vec3 cameraRight = mMainCamera->transform.GetRight();
	glm::vec3 cameraForward = mMainCamera->transform.GetForward();

	cameraRight.y = 0;
	cameraForward.y = 0;
	cameraRight = glm::normalize(cameraRight);
	cameraForward = glm::normalize(cameraForward);
	
	mHead->mCurrentPosition += cameraForward * mMoveDir.y * deltaTime * mSpeed;
	mHead->mCurrentPosition += cameraRight * mMoveDir.x * deltaTime * mSpeed;
	mHead->mCurrentPosition.y += mUpAxis * deltaTime * mSpeed;
}

void Snake::SetInput()
{
	mUpAxis = 0;

	mUpAxis += mUpHeld ? 1 : 0;
	mUpAxis -= mDownHeld ? 1 : 0;


	mMoveDir.x = InputManager::GetInstance().GetAxisX();
	mMoveDir.y = InputManager::GetInstance().GetAxisY();

	if (glm::length(mMoveDir) > 0)
	{
		mMoveDir = glm::normalize(mMoveDir);
	}
}
