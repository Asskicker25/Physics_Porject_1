#pragma once
#include "PhysicsEngineThreadInfo.h"
#include <Windows.h>

extern DWORD WINAPI UpdatePhysicsEngine(LPVOID lpParameter);
extern PhysicsEngineThreadInfo* InitializePhysicsThread(float fixedStepTime);