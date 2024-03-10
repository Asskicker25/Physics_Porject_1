#include "FlagMaterial.h"
#include "../Flag/Flag.h"

using namespace Verlet;

void FlagMaterial::UpdateMaterial(Shader* shader)
{
	mFlag->flagShader->Bind();

	//mFlag->flagShader->SetUniform1i("numOfNodes", (int)mFlag->mListOfBulletHoleNode.size());
	//mFlag->flagShader->SetUniform1f("holeRadius", mFlag->mBulletHoleRadius);
	

	//int i = 0;
	//for (Flag::Node* node : mFlag->mListOfBulletHoleNode)
	//{
	//	if (i > 99) break;
	//	std::string str = std::to_string(i);

	//	mFlag->flagShader->SetUniform3f("holeNodes[" + str + "].position",
	//		node->mCurrentPosition.x, node->mCurrentPosition.y, node->mCurrentPosition.z);
	//	i++;
	//}

	mFlag->flagShader->Unbind();

	Material::UpdateMaterial(shader);

}

void FlagMaterial::SetFlag(Flag* flag)
{
	mFlag = flag;
}
