#include "BaseSoftBody.h"
#include <Graphics/Renderer.h>
#include <Graphics/Panels/ImguiDrawUtils.h>

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

