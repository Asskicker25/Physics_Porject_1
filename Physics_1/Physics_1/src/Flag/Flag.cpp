#include "Flag.h"
#include "../Material/FlagMaterial.h"


Flag::Flag()
{
	LoadShader();
	name = "Flag";
	mGravity = glm::vec3(0.0, -5, 0.0f);
	mTightness = 0.5f;
	showDebugModels = false;

	LoadModel("Assets/Models/Flag.ply");
	shader = flagShader;
	meshes[0]->material = new FlagMaterial();
	FlagMaterial* mat = (FlagMaterial*)meshes[0]->material;
	mat->diffuseTexture = new Texture("Assets/Models/SpaceMarine.png");
	mat->alphaMask = new Texture("Assets/Models/SpaceMarine_Alpha.png");
	mat->useMaskTexture = true;
	mat->SetFlag(this);
	transform.SetPosition(glm::vec3(-5.5f, 14.5f, -10.0f));
	transform.SetRotation(glm::vec3(0.0f, 0.0f, 90.0f));
	transform.SetScale(glm::vec3(40.0f, 30.0f, 1.0f)); 

	mNodeMaxVelocity = glm::vec3(0, -50, 0);
	clampVelocity = true;

	AddLockNode(glm::vec3(0, 0.0f, 0.0f), 0.4f);
	AddLockNode(glm::vec3(-4.5f, 0.0f, 0.0f), 0.4f);
	AddLockNode(glm::vec3(-9, 0.0f, 0.0f), 0.4f);

}

Flag::~Flag()
{
	while (mListOfSticks.size() != 0)
	{
		delete mListOfSticks[0];
		mListOfSticks.erase(mListOfSticks.begin());
	}

	mListOfSticks.clear();

	while (mListOfNodes.size() != 0)
	{
		delete mListOfNodes[0];
		mListOfNodes.erase(mListOfNodes.begin());
	}

	mListOfNodes.clear();
}

void Flag::Start()
{
	InitializeSoftBody();
}

void Flag::Update(float deltaTime)
{
	HandleTear();
}

void Flag::HandleTear()
{
	if (mChain->mTail == nullptr) return;

	glm::vec3 tailPos = mChain->mTail->mCurrentPosition;

	for (Node* node : mListOfNodes)
	{
		if (!node->mIsConnected) continue;

		glm::vec3 diff = node->mCurrentPosition - tailPos;

		float sqDist = glm::dot(diff, diff);


		if (sqDist < mChain->mNodeRadius * mChain->mNodeRadius)
		{
			CreateHole(node);

			return;
			
		}
	}
}

void Flag::LoadShader()
{
	flagShader = new FlagShader();
	flagShader->LoadShader("res/Shader/FlagShader.shader", Shader::ALPHA_CUTOUT);
}

void Flag::CreateHole(Node* node)
{
	std::vector<Node*> listOfNodesToDisconnect;

	for (Node* adjacent : mListOfNodes)
	{
		glm::vec3 diff = node->mCurrentPosition - node->mCurrentPosition;

		float sqDist = glm::dot(diff, diff);

		if (sqDist > mBulletHoleRadius * mBulletHoleRadius) continue;

		listOfNodesToDisconnect.push_back(node);
	}

	listOfNodesToDisconnect.push_back(node);

	for (Node* disconnectNode : listOfNodesToDisconnect)
	{
		for (Stick* stick : disconnectNode->mListOfConnectedSticks)
		{
			DisconnectStick(stick);
		}

		disconnectNode->mIsConnected = false;

		MakeNodeInvisible(disconnectNode);
	}

}



void Flag::MakeNodeInvisible(Node* node)
{
	for (MeshAndMaterial* mesh : meshes)
	{
		for (unsigned int& index : node->mListOfIndexs)
		{
			mesh->mesh->vertices[index].enabled = 1;
		}
	}
}
