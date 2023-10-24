#pragma once

#include "Debugger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Model.h"

class Renderer
{
private :
	std::vector<Model*> models;
	std::vector<Shader*> shaders;
public:
	void Clear();
	void AddModel(Model* model, Shader* shader);
	void AddModel(Model& model, Shader& shader);
	void Draw();
};

