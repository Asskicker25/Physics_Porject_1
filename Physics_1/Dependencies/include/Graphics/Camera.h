#pragma once

#include "Debugger.h"

enum ECameraType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera
{
private:
	ECameraType cameraType;
	float cameraWidth;
	float cameraHeight;
	float nearPlane;
	float farPlane;
	float fov;

	glm::mat4 cameraMatrix;

	void SetCameraProjection();

public:
	Camera();
	Camera(ECameraType _cameraType,float _cameraWidth,float _cameraHeight,float _nearPlane,float _farPlane, float _fov);
	Camera(ECameraType _cameraType,float _cameraWidth,float _cameraHeight,float _nearPlane,float _farPlane);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane, float _fov);
	void InitializeCamera(ECameraType _cameraType, float _cameraWidth, float _cameraHeight, float _nearPlane, float _farPlane);
	void ChangeCameraType(ECameraType type);

	inline glm::mat4 GetMatrix() const { return cameraMatrix; }

};

