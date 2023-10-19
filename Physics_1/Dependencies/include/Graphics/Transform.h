#pragma once

#include "Debugger.h"

class Transform
{

public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform();
	Transform(const Transform& transform);

	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	glm::mat4 GetTransformMatrix();
	glm::mat4 GetInverseMatrix();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
};

