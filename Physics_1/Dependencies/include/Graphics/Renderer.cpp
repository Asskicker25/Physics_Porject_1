#include "Renderer.h"

void Renderer::Clear()
{
	GLCALL(glClearColor(0.1f, 0.3f, 0.4f, 1.0f));

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::AddModel(Model* model, Shader* shader)
{
	models.push_back(model);
	shaders.push_back(shader);
}

void Renderer::AddModel(Model& model, Shader& shader)
{
	models.push_back(&model);
	shaders.push_back(&shader);
}

void Renderer::Draw()
{
	for (unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(*shaders[i]);
	}
}
