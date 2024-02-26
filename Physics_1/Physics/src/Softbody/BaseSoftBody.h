#pragma once
#include <Graphics/Model.h>
#include <Windows.h>

class BaseSoftBody : public Model
{
public:
	virtual void UpdateSoftBody(float deltaTime, CRITICAL_SECTION& criticalSection) = 0;
	virtual void UpdateBufferData() = 0;
};

