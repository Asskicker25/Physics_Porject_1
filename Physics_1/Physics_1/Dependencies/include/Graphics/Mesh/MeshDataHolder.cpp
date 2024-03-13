#include "MeshDataHolder.h"

MeshDataHolder::~MeshDataHolder()
{
	delete diffuseTexture;
	delete specularTexture;
	delete alphaTexture;
}

ImageHolder::~ImageHolder()
{
	delete image.pixelData;
}
