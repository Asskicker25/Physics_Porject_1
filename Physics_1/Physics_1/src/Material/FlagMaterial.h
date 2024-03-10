#pragma once
#include <Graphics/Material/Material.h>

class Flag;

class FlagMaterial : public Material
{
public:

	void UpdateMaterial(Shader* shader) override;
	void SetFlag(Flag* flag);

private:
	Flag* mFlag = nullptr;
};

