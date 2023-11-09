#pragma once

#include "Debugger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"

struct ModelAndShader
{
public:
	Model* model;
	Shader* shader;
};

class Renderer
{
private :
	std::vector<Model*> models;
	std::vector<Shader*> shaders;
	std::vector<ModelAndShader*> modelAndShaders;

	glm::vec3 backGroundColor = glm::vec3(0.1f, 0.3f, 0.4f);

public:
	void Clear();
	void AddModel(Model* model, Shader* shader);
	void AddModel(Model& model, Shader& shader);
	void RemoveModel(Model* model);
	void RemoveModel(Model& model);
	void SetBackgroundColor(const glm::vec3& backGroundColor);
	void Draw();
};

