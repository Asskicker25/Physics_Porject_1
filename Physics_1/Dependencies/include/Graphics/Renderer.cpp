#include "Renderer.h"

void Renderer::Clear()
{
	GLCALL(glClearColor(backGroundColor.x, backGroundColor.y, backGroundColor.z, 1.0f));

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::AddModel(Model* model, Shader* shader)
{
	/*models.push_back(model);
	shaders.push_back(shader);*/

	modelAndShaders.push_back( new  ModelAndShader{ model,shader } );
}

void Renderer::AddModel(Model& model, Shader& shader)
{
	/*models.push_back(&model);
	shaders.push_back(&shader);*/

	modelAndShaders.push_back(new  ModelAndShader{ &model,&shader });

}

void Renderer::RemoveModel(Model* model)
{
	//models.erase(std::remove())


	for (ModelAndShader* modelShader : modelAndShaders)
	{
		if (modelShader->model == model)
		{
			modelAndShaders.erase(std::remove(modelAndShaders.begin(), modelAndShaders.end(), modelShader),modelAndShaders.end());
			return;
		}
	}
}

void Renderer::RemoveModel(Model& model)
{

	for (ModelAndShader* modelShader : modelAndShaders)
	{
		if (modelShader->model == &model)
		{
			modelAndShaders.erase(std::remove(modelAndShaders.begin(), modelAndShaders.end(), modelShader), modelAndShaders.end());
			return;
		}
	}
}

void Renderer::SetBackgroundColor(const glm::vec3& backGroundColor)
{
	this->backGroundColor = backGroundColor;
}

void Renderer::Draw()
{
	for (unsigned int i = 0; i < modelAndShaders.size(); i++)
	{
		modelAndShaders[i]->model->Draw(modelAndShaders[i]->shader);
		//models[i]->Draw(*shaders[i]);
	}
}
