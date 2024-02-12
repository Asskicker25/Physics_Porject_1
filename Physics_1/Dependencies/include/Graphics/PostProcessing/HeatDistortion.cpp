#include "HeatDistortion.h"
#include "../Timer.h"

void PostProcessing_v1::HeatDistortion::OnPropertyDraw()
{
	if (!ImGui::TreeNodeEx(mName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		return;
	}


	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 150);
	ImGui::Text("Amount");
	ImGui::NextColumn();


	ImGui::TreePop();
}

void PostProcessing_v1::HeatDistortion::SetShaderUniforms()
{
	shader->SetUniform1f("iTime", Timer::GetInstance().elapsedTime);
}
