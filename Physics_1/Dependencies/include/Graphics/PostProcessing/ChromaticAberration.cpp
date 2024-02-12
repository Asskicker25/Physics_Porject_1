#include "ChromaticAberration.h"
#include "../Timer.h"

using namespace PostProcessing_v1;

void ChromaticAberration::SetShaderUniforms()
{
	shader->SetUniform1f("iTime", Timer::GetInstance().elapsedTime);
	shader->SetUniform1f("displaceAmount", mAmount);
}

void ChromaticAberration::OnPropertyDraw()
{

	if (!ImGui::TreeNodeEx(mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}


	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 150);
	ImGui::Text("Amount");
	ImGui::NextColumn();

	ImGui::InputFloat("###Chromatic amount", &mAmount);

	ImGui::TreePop();

	ImGui::Columns(1);
}

