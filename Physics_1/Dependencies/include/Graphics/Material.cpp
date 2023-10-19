#include "Material.h"

Material::Material()
{
	ambientType = AmbientType::Value;
	ambientColor = glm::vec3(1.0f);
	ResetMaterial();
}

Material::Material(const Material& material)
{
	SetBaseColor(glm::vec3(material.baseColor.x, material.baseColor.y, material.baseColor.z));
	SetAmbientColor(glm::vec3(material.ambientColor.x, material.ambientColor.y, material.ambientColor.z));
	diffuseTexture = material.diffuseTexture;
	specularTexture = material.specularTexture;

	ambientType = material.ambientType;
	ambientValue = material.ambientValue;
	specularValue = material.specularValue;
	shininess = material.shininess;
}

Material::~Material()
{
}

void Material::ResetMaterial()
{
	baseColor = glm::vec3(1.0f);
	ambientValue = 1.0f;
	specularValue = 0.5f;
}

float Material::GetSpecularValue()
{
	return specularValue;
}

glm::vec3 Material::GetBaseColor()
{
	return baseColor;
}

glm::vec3 Material::GetAmbientColor()
{
	switch (ambientType)
	{
	case AmbientType::Color:
		return ambientColor;
	case AmbientType::Value:
		return baseColor * ambientValue;
	}
}

void Material::SetBaseColor(glm::vec3 color)
{
	baseColor = color;
}

void Material::SetAmbientColor(glm::vec3 color)
{
	ambientColor = color;
}


