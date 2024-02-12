#pragma once
#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>


extern void DrawVector3ImGui(std::string label, glm::vec3& values, float resetValue, float& columnWidth);
