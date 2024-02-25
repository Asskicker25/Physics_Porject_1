#pragma once
#include <Graphics/Model.h>

class BaseSoftBody : public Model
{
public:
	virtual void UpdateSoftBody(float deltaTime) = 0;
};

