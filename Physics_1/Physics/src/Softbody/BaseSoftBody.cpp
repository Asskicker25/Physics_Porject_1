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
		Renderer::GetInstance().DrawSphere(node->mCurrentPosition, mNodeRadius, nodeColor);
	}


	for (Stick* stick : mListOfSticks)
	{
		Renderer::GetInstance().DrawLine(stick->mNodeA->mCurrentPosition, stick->mNodeB->mCurrentPosition, stickColor);
	}
}

void BaseSoftBody::OnPropertyDraw()
{
	Model::OnPropertyDraw();

	if (!ImGui::TreeNodeEx("SoftBody", ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}

	ImGuiUtils::DrawBool("ShowDebug", showDebugModels);

	ImGui::TreePop();

}

