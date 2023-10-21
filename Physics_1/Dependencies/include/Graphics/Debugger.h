#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSERT(x)	if(x == true) __debugbreak();
#define GLCALL(x)  \
    if (glfwGetCurrentContext())  \
    {  \
        ClearErrors();  \
        x;  \
        ASSERT(GetError(#x, __FILE__, __LINE__));  \
    }


void ClearErrors();
bool GetError(const char* function, const char* file, int line);

class Debugger
{
public:
	static void Print(const char* message);
	static void Print(const std::string& message);
	static void Print(const int& message);
	static void Print(const glm::vec3& message);
};

