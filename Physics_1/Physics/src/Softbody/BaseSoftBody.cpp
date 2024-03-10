#include "BaseSoftBody.h"
#include <Graphics/Renderer.h>
#include <Graphics/Panels/ImguiDrawUtils.h>
#include <Graphics/MathUtils.h>

using namespace MathUtilities;

void BaseSoftBody::CleanZeros(glm::vec3& value)
{
	const float minFloat = 1.192092896e-07f;
	if ((value.x < minFloat) && (value.x > -minFloat))
	{
		value.x = 0.0f;
	}
	if ((value.y < minFloat) && (value.y > -minFloat))
	{
		value.y = 0.0f;
	}
	if ((value.z < minFloat) && (value.z > -minFloat))
	{
		value.z = 0.0f;
	}
}


void BaseSoftBody::Render()
{
	if (!showDebugModels) return;

	for (Node* node : mListOfNodes)
	{
		Renderer::GetInstance().DrawSphere(node->mCurrentPosition, node->mRadius, nodeColor);
	}


	for (Stick* stick : mListOfSticks)
	{
		if (!stick->isConnected) continue;

		Renderer::GetInstance().DrawLine(stick->mNodeA->mCurrentPosition, stick->mNodeB->mCurrentPosition, stickColor);
	}
}

void BaseSoftBody::AddCollidersToCheck(PhysicsObject* phyObj)
{
	mListOfCollidersToCheck.push_back(phyObj);
}

void BaseSoftBody::SetNodeRadius(int index, float radius)
{
	mListOfNodes[index]->mRadius = radius;
}

void BaseSoftBody::DisconnectStick(Stick* stick)
{
	stick->isConnected = false;
}

void BaseSoftBody::OnPropertyDraw()
{
	Model::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("SoftBody", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawBool("ShowDebug", showDebugModels);
	ImGuiUtils::DrawFloat("BounceFactor", mBounceFactor);

	ImGui::TreePop();

}

bool BaseSoftBody::ShouldApplyGravity(Node* node)
{
	for (Node* noGravNode : mListOfNonGravityNodes)
	{
		if (node == noGravNode)
		{
			return false;
		}
	}

	return true;
}

void BaseSoftBody::UpdateSoftBody(float deltaTime)
{
	UpdateNodePosition(deltaTime);
	UpdatePositionByVerlet(deltaTime);
	ApplyCollision(deltaTime);
	SatisfyConstraints(deltaTime);
	UpdateModelData(deltaTime);
}


void BaseSoftBody::UpdateNodePosition(float deltaTime)
{
	EnterCriticalSection(mCriticalSection);

	for (Node* node : mListOfNodes)
	{
		if (node->mIsLocked) continue;


		if (ShouldApplyGravity(node))
		{
			node->velocity += mGravity * deltaTime;
		}

		if (clampVelocity)
		{
			node->velocity.x = glm::clamp(node->velocity.x, -mNodeMaxVelocity.x, mNodeMaxVelocity.x);
			node->velocity.y = glm::clamp(node->velocity.y, -mNodeMaxVelocity.y, mNodeMaxVelocity.y);
			node->velocity.z = glm::clamp(node->velocity.z, -mNodeMaxVelocity.z, mNodeMaxVelocity.z);
		}

		//node->mCurrentPosition += node->velocity * deltaTime;


		//UpdatePositionByVerlet(node, deltaTime);
	}

	LeaveCriticalSection(mCriticalSection);

}

void BaseSoftBody::UpdatePositionByVerlet(float deltaTime)
{
	for (Node* node : mListOfNodes)
	{
		if (node->mIsLocked) continue;

		glm::vec3 posBeforUpdate = node->mCurrentPosition;

		if (node->mIsColliding)
		{
			node->mCurrentPosition += (node->velocity * deltaTime);
		}
		else
		{
			node->mCurrentPosition += (posBeforUpdate - node->mOldPositionm) + (node->velocity * (deltaTime * deltaTime));
		}

		node->mOldPositionm = posBeforUpdate;

		CleanZeros(node->mCurrentPosition);
		CleanZeros(node->mOldPositionm);
	}

}

void BaseSoftBody::SatisfyConstraints(float deltaTime)
{

	for (unsigned int i = 0; i < mNumOfIterations; i++)
	{
		for (Stick* stick : mListOfSticks)
		{
			if (!stick->isConnected) continue;

			Node* nodeA = stick->mNodeA;
			Node* nodeB = stick->mNodeB;

			glm::vec3 delta = nodeB->mCurrentPosition - nodeA->mCurrentPosition;
			float length = glm::length(delta);

			float diff = (length - stick->mRestLength) / length;

			if (!nodeA->mIsLocked && !nodeA->mIsColliding)
			{
				nodeA->mCurrentPosition += delta * 0.5f * diff * mTightness;
			}

			if (!nodeB->mIsLocked && !nodeB->mIsColliding)
			{
				nodeB->mCurrentPosition -= delta * 0.5f * diff * mTightness;
			}

			/*glm::vec3 stickCenter = (nodeA->mCurrentPosition + nodeB->mCurrentPosition) / 2.0f;
			glm::vec3 stickDir = glm::normalize((nodeA->mCurrentPosition - nodeB->mCurrentPosition));

			if (!nodeA->mIsLocked)
			{
				nodeA->mCurrentPosition = stickCenter + stickDir * stick->mRestLength * 0.5f;
			}

			if (!nodeB->mIsLocked)
			{
				nodeB->mCurrentPosition = stickCenter - stickDir * stick->mRestLength * 0.5f;
			}*/

			CleanZeros(nodeA->mCurrentPosition);
			CleanZeros(nodeB->mCurrentPosition);
		}
	}
}


void BaseSoftBody::UpdateModelData(float deltaTime)
{
	UpdateModelVertices();
	UpdateModelNormals();
}

void BaseSoftBody::CreateRandomBracing(int numOfBraces, float distanceBetween)
{
	int size = mListOfNodes.size() - 1;

	for (int i = 0; i < numOfBraces; i++)
	{
		float sqDist = 0;

		Node* nodeA = nullptr;
		Node* nodeB = nullptr;

		do
		{
			nodeA = mListOfNodes[MathUtils::GetRandomIntNumber(0, size)];
			nodeB = mListOfNodes[MathUtils::GetRandomIntNumber(0, size)];

			glm::vec3 diff = nodeB->mCurrentPosition - nodeA->mCurrentPosition;
			sqDist = glm::dot(diff, diff);

		} while (sqDist < distanceBetween * distanceBetween);

		Stick* newStick = new Stick(nodeA, nodeB);
		mListOfSticks.push_back(newStick);
	}
}


void BaseSoftBody::UpdateBufferData()
{
	for (MeshAndMaterial* mesh : meshes)
	{
		mesh->mesh->UpdateVertices();
	}
}


void BaseSoftBody::ApplyCollision(float deltaTime)
{

	//for (Node* node : mListOfNodes)
	//{
	//	if (node->mCurrentPosition.y < -5)
	//	{
	//		node->mCurrentPosition.y = -5;
	//		//node->mOldPositionm = node->mCurrentPosition;
	//	}
	//}

	//return;


	std::vector<glm::vec3> collisionPts, collisionNr;

	for (Node* node : mListOfNodes)
	{
		node->mIsColliding = false;
	}


	/*for (Node* node : mListOfNodes)
	{
		if (node->mCurrentPosition.y < -5)
		{
			node->mIsColliding = true;
			node->mCurrentPosition.y = -5;
		}
	}

	return;*/

	for (PhysicsObject* phyObj : mListOfCollidersToCheck)
	{
		int numOfCollisions = 0;

		for (Node* node : mListOfNodes)
		{
			bool nodeCollided = false;

			Sphere nodeSphere(node->mCurrentPosition, node->mRadius);

			if (collisionMode == TRIGGER) continue;

			collisionPts.clear();
			collisionNr.clear();

			switch (phyObj->shape)
			{
			case SPHERE:

				if (CollisionSphereVSSphere(&nodeSphere, (Sphere*)phyObj->transformedPhysicsShape, collisionPts, collisionNr))
				{
					numOfCollisions++;
					nodeCollided = true;
				}
				break;

			case AABB:

				if (CollisionSpherevsAABB(&nodeSphere, phyObj->GetModelAABB(), true, collisionPts, collisionNr))
				{
					numOfCollisions++;
					nodeCollided = true;
				}

				break;

			}

			if (!nodeCollided) continue;

			EnterCriticalSection(mCriticalSection);

			glm::vec3 normal = glm::vec3(0.0f);
			glm::vec3 collisionPt = glm::vec3(0.0f);

			for (size_t i = 0; i < collisionNr.size(); i++)
			{
				if (glm::length(collisionNr[i]) == 0)
				{
					normal += (collisionNr[i]);
				}
				else
				{
					normal += glm::normalize(collisionNr[i]);
				}
			}

			for (size_t i = 0; i < collisionPts.size(); i++)
			{

				collisionPt += (collisionPts[i]);
			}

			normal = normal / (float)collisionNr.size();
			collisionPt = collisionPt / (float)collisionPts.size();

			glm::vec3 reflected = glm::reflect(glm::normalize(node->velocity), normal);
			node->velocity = reflected * glm ::length(node->velocity ) * 0.5f;
			node->velocity *= mBounceFactor;
			//node->velocity = glm::vec3(0);

			//node->mCurrentPosition = collisionPt + ( reflected * node->mRadius);
			node->mIsColliding = true;
			//node->mOldPositionm = node->mCurrentPosition;

			LeaveCriticalSection(mCriticalSection);
		}

	}
}
