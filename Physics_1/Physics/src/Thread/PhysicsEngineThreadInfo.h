#pragma once
#include <Windows.h>
#include "../PhysicsEngine.h"

struct PhysicsEngineThreadInfo
{
    PhysicsEngine* physicsEngine;

    double desiredUpdatetime = 0.0;
    bool isRunning = false;
    bool isAlive = true;
    bool isCalculationsDone = false;
    bool isCalculating = false;

    DWORD sleepTime = 0;
    DWORD ThreadId = 0;
    HANDLE threadHandle = 0;

    CRITICAL_SECTION softBodyUpdateModelData;
};

