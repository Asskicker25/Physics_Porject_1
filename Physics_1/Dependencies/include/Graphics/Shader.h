#pragma once
#include "Debugger.h"


enum ShaderType
{
	NONE = -1,
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER = 1
};

struct ShaderSource
{
	std::string vertexSrc;
	std::string fragmentSrc;
};

class Shader
{
private:
	unsigned int shaderId;
	std::unordered_map <std::string, int> cachedLocations;
	int GetLocation(const std::string& property);

public:
	Shader();
	Shader(const std::string& path);
	~Shader();

	void LoadShader(const std::string& path);
	void Bind();
	void Unbind();
	unsigned int GetShaderId();

	void SetUniform1f(const std::string& property, float x);
	void SetUniform3f(const std::string& property, float x, float y, float z);
	void SetUniform4f(const std::string& property, float x, float y, float z, float w);
	void SetUniform1i(const std::string& property,int slot);
	void SetUniformMat(const std::string& property, glm::mat4 value);
};

