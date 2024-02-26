#include "PhysicsEngineThread.h"


DWORD UpdatePhysicsEngine(LPVOID lpParameter)
{
	PhysicsEngineThreadInfo* threadInfo = (PhysicsEngineThreadInfo*)lpParameter;

	double currentTime = glfwGetTime();
	double lastTime = currentTime;
	double deltaTime = 0.0f;

	double timeStep = 0.0f;

	while (threadInfo->isAlive)
	{
		if (threadInfo->isRunning)
		{
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			if (deltaTime > threadInfo->desiredUpdatetime) { deltaTime = threadInfo->desiredUpdatetime; }

			timeStep += deltaTime;

			if (timeStep >= threadInfo->desiredUpdatetime)
			{
				timeStep = 0;

				threadInfo->physicsEngine->UpdateSoftBodies(deltaTime, threadInfo->softBodyUpdateModelData);
			}

			Sleep(threadInfo->sleepTime);
		}
		
	}


	return 0;
}

PhysicsEngineThreadInfo* InitializePhysicsThread(float fixedStepTime)
{
	PhysicsEngineThreadInfo* threadInfo = new PhysicsEngineThreadInfo();
	threadInfo->desiredUpdatetime = fixedStepTime;
	threadInfo->physicsEngine = &PhysicsEngine::GetInstance();
	threadInfo->isAlive = true;
	threadInfo->sleepTime = 1;

	InitializeCriticalSection(&threadInfo->softBodyUpdateModelData);
	

	threadInfo->threadHandle = CreateThread(
		NULL,						// lpThreadAttributes,
		0,							// dwStackSize,
		UpdatePhysicsEngine,		// lpStartAddress,
		(void*)threadInfo,			//  lpParameter,
		0,							// dwCreationFlags (0 or CREATE_SUSPENDED)
		&threadInfo->ThreadId);		// lpThreadId


	return threadInfo;
}
