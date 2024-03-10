#pragma once

#include <Softbody/SoftBodyForMeshes.h>
#include <PhysicsObject.h>
#include <Graphics/Camera/GameCamera.h>
#include <Graphics/InputManager/InputManager.h>


using namespace Verlet;


class Chain : public SoftBodyForMeshes, public iInputListener
{
public:

	Chain(std::vector<PhysicsObject*>& listOfPhysicsObject);

	virtual void OnKeyPressed(const int& key);
	virtual void OnKeyReleased(const int& key);

	virtual void Update(float deltaTime) override;


private:

	void HandleMove(float deltaTime);
	void HandleCamera();
	void SetInput();
	
	float mSpeed = 5;
	float mUpAxis = 0;
	float mCamDistance = 0;
	float mCamYOffset = 0;

	bool mUpHeld = false;
	bool mDownHeld = false;

	glm::vec2 mMoveDir = glm::vec2(0);
	Node* mHead = nullptr;

	Camera* mMainCamera = nullptr;
};

