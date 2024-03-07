#pragma once

#include "BasePanel.h"
#include "../Buffer/FrameBuffer.h"

class GameViewportPanel : public BasePanel
{

public:
	// Inherited via BasePanel
	void OnRender(float windowSizeX, float windowSizeY) override;

	FrameBuffer* sceneBuffer = nullptr;

	ImVec2 mViewportSize;
	ImVec2 mResolution;
	ImVec4 windowColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

};

