#include "Debugger.h"
#include <sstream>


void ClearErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

bool GetError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL ERROR [" << error << "] : " << function << " " << file << " : " << line << std::endl;
		return true;
	}
	return false;
}


void Debugger::Print(const char* message)
{
	std::cout << message << std::endl;
}

void Debugger::Print(const std::string& message)
{
	std::cout << message << std::endl;
}

void Debugger::Print(const int& message)
{
	std::cout << message << std::endl;
}

void Debugger::Print(const glm::vec3& message)
{
	std::cout << message.x << ",";
	std::cout << message.y << ",";
	std::cout << message.z << std::endl;
}


