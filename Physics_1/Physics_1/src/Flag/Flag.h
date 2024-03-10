#pragma once

#pragma once
#include <Softbody/SoftBodyForVertex.h>
#include <Graphics/InputManager/InputManager.h>
#include "../Shader/FlagShader.h"

#include "../Player/Chain.h"

using namespace Verlet;

class Flag : public SoftBodyForVertex
{
public:

	Flag();
	~Flag();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	float mBulletHoleRadius = 0.5f;

	FlagShader* flagShader = nullptr;

	Chain* mChain = nullptr;

private:

	void HandleTear();
	void MakeNodeInvisible(Node* node);
	void LoadShader();
	void CreateHole(Node* node);
};

