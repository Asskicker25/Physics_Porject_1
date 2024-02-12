#pragma once

#include "Camera.h"

class CameraSystem
{
public:

	static CameraSystem& GetInstance();

	void AddCamera(Camera* camera);
	void RemoveCamera(Camera* camera);

	Camera* GetMainCamera();

	std::vector<Camera*> mListOfCameras;

};

