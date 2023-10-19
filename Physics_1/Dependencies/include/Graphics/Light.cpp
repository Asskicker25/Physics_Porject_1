#include "Light.h"

Light::Light(Model& model, LightType newlightType) : lightType{ newlightType }, transform {nullptr}
{
	InitializeLight(model, newlightType);
}

Light::Light()
{

}

Light::~Light()
{
}

void Light::InitializeLight(Model& model, LightType lightType)
{
	this->lightType = lightType;
	lightModel = &model;
	intensity = 0.5f;
	transform = &(lightModel->transform);
	attenuation = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	innerAngle = 30;
	outerAngle = 50;
}

glm::vec3 Light::GetLightColor()
{
	return material.GetBaseColor() * intensity;
}

glm::vec3 Light::GetAmbientColor()
{
	return material.GetAmbientColor() * intensity;
}

//glm::vec3 Light::GetSpecularColor()
//{
//	return glm::vec3(material.specularValue);
//}

float Light::GetAmbientValue()
{
	return material.ambientValue;
}

//float Light::GetSpecularValue()
//{
//	return material.specularValue;
//}

void Light::SetLightColor(glm::vec3 color)
{
	material.SetBaseColor(color);
}

void Light::SetAmbientColor(glm::vec3 color)
{
	material.SetAmbientColor(color);
}

void Light::SetAmbientValue(float ambientValue)
{
	material.ambientValue = ambientValue;
}

//void Light::SetSpecualrValue(float specularValue)
//{
//	material.specularValue = specularValue;
//}

