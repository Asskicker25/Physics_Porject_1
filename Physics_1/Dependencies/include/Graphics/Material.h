
#pragma once

#include "Debugger.h"
#include "Texture.h"

enum AmbientType
{
	Value,
	Color
};

class Material
{

private:
	glm::vec3 baseColor;
	glm::vec3 ambientColor;

public:
	Texture* diffuseTexture;
	Texture* specularTexture;
	AmbientType ambientType;

	float ambientValue;
	float specularValue;
	float shininess = 32;

	Material();
	Material(const Material& material);
	~Material();

	void ResetMaterial();
	float GetSpecularValue();
	glm::vec3 GetBaseColor();
	glm::vec3 GetAmbientColor();

	void SetBaseColor(glm::vec3 color);
	void SetAmbientColor(glm::vec3 color);
};


