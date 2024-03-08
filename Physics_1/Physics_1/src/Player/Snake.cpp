#include "Snake.h"

Snake::Snake(std::vector<PhysicsObject*>& listOfPhysicsObject)
{
	LoadModel("Assets/Models/Rope_2.fbx");
	transform.SetRotation(glm::vec3(0, 0, 0));
	transform.SetScale(glm::vec3(0.5f));
	mGravity = glm::vec3(0, -1, 0);
	//softbodyMesh->showDebugModels = false;
	mNodeRadius = 0.5f;
	InitializeLockNodes({ 0 });
	InitializeSoftBody();
	LockNodeAtIndex(0);
	mBounceFactor = 1.0f;

	for (PhysicsObject* phy : listOfPhysicsObject)
	{
		AddCollidersToCheck(phy);
	}

}
