#include "Chain.h"
#include <Graphics/Camera/CameraSystem.h>

Chain::Chain(std::vector<PhysicsObject*>& listOfPhysicsObject)
{
	LoadModel("Assets/Models/Chain.fbx");
	transform.SetPosition(glm::vec3(0, 10, 0));
	transform.SetRotation(glm::vec3(0, 0, 0));
	transform.SetScale(glm::vec3(0.5f));
	mGravity = glm::vec3(0, -1, 0);
	//softbodyMesh->showDebugModels = false;
	mNodeRadius = 0.5f;
	InitializeLockNodes({ 0 });
	LockNodeAtIndex(0);
	mBounceFactor = 1.5f;

	for (PhysicsObject* phy : listOfPhysicsObject)
	{
		AddCollidersToCheck(phy);
	}


	mMainCamera = CameraSystem::GetInstance().GetMainCamera();
	mCamDistance = glm::length(mMainCamera->transform.position - transform.position);
	mCamYOffset = mMainCamera->transform.position.y - transform.position.y;

	for (MeshAndMaterial* mesh : meshes)
	{
		mesh->material->AsMaterial()->SetBaseColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	}

	InputManager::GetInstance().AddListener(this);
}

void Chain::OnKeyPressed(const int& key)
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

void Chain::OnKeyReleased(const int& key)
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

void Chain::Update(float deltaTime)
{
	Model::Update(deltaTime);
	SetInput();
	HandleMove(deltaTime);
	HandleCamera();

}

void Chain::Start()
{
	InitializeSoftBody();
	mHead = mListOfNodes[0];
	mTail = mListOfNodes[mListOfNodes.size() - 1];

}

void Chain::SetInput()
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

void Chain::HandleMove(float deltaTime)
{
	glm::vec3 cameraRight = mMainCamera->transform.GetRight();
	glm::vec3 cameraUp = mMainCamera->transform.GetUp();

	cameraRight.y = 0;
	cameraUp.y = 0;
	cameraRight = glm::normalize(cameraRight);
	cameraUp = glm::normalize(cameraUp);
	
	mHead->mCurrentPosition += cameraUp * mMoveDir.y * deltaTime * mSpeed;
	mHead->mCurrentPosition += cameraRight * mMoveDir.x * deltaTime * mSpeed;
	mHead->mCurrentPosition.y += mUpAxis * deltaTime * mSpeed;
}


void Chain::HandleCamera()
{
	glm::vec3 pos = mHead->mCurrentPosition;
	pos.y += mCamYOffset;
	mMainCamera->transform.SetPosition(pos - transform.GetForward() * mCamDistance);
}
