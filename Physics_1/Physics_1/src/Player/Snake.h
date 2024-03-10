#pragma once

#include <Softbody/SoftBodyForMeshes.h>
#include <PhysicsObject.h>
#include <Graphics/Camera/GameCamera.h>
#include <Graphics/InputManager/InputManager.h>


using namespace Verlet;


class Snake : public SoftBodyForMeshes, public iInputListener
{
public:

	Snake(std::vector<PhysicsObject*>& listOfPhysicsObject);

	virtual void OnKeyPressed(const int& key);
	virtual void OnKeyReleased(const int& key);

	virtual void Update(float deltaTime) override;


private:
	void HandleMove(float deltaTime);
	void HandleCamera();
	void SetInput();
	
	float mSpeed = 5;
	float mUpAxis = 0;

	bool mUpHeld = false;
	bool mDownHeld = false;

	glm::vec2 mMoveDir = glm::vec2(0);
	Node* mHead = nullptr;

	Camera* mMainCamera = nullptr;
};

