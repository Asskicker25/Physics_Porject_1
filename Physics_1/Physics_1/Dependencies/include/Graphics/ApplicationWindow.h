#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui_internal.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/ImGuizmo.h>

#include "Renderer.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/VertexLayout.h"
#include "Buffer/IndexBuffer.h"
#include "Shaders/Shader.h"
#include "Texture/Texture.h"
#include "Texture/RenderTexture.h"
#include "Transform.h"
#include "Camera/Camera.h"
#include "Mesh/Model.h"
#include "Light/Light.h"
#include "Light/LightManager.h"

#include "Material/Material.h"
#include "Material/UnlitColorMaterial.h"
#include "Texture/CubeMapTexture.h"
#include "Material/SkyBoxMaterial.h"
#include "Mesh/DebugCubeData.h"
#include "Mesh/DebugLineData.h"
#include "Timer.h"
#include "Panels/PanelManager.h"
#include "Panels/EditorLayout.h"
#include "Buffer/FrameBuffer.h"
#include "Shaders/UnlitColorShader.h"
#include "Shaders/SkyBoxShader.h"
#include "Camera/GameCamera.h"
#include "PostProcessing/PostProcessing.h"
#include "Mesh/RenderQuad.h"
#include "Scene/SceneManager.h"
#include "ParticleSystem/ParticleSystemManager.h"


class ApplicationWindow
{
private:

	const char* glsl_version = "#version 130";

	GLFWwindow* window;

	void MoveMouse();
	void MoveCameraKeyBoard(GLFWwindow* window);
	void SetViewportSize(GLFWwindow* window, int width, int height);
	void GetCursorCallback(GLFWwindow* window, double xpos, double ypos);

	void RenderForCamera(Camera* camera, FrameBuffer* frameBuffer, bool viewport = false);

public:
	float moveSpeed{ 10.0f };
	float mouseSens{ 0.1f };

	float windowWidth{ 1280 };
	float windowHeight{ 720 };

	float gameResolutionWidth{  1280 };
	float gameResolutionHeight{  720 };

	bool mouseHeld{ false };
	bool mouseCameraMove{ false };
	bool stopKeyCallback{ false };
	bool stopMouseCallback{ false };
	bool applicationPlay{ false };
	bool imGuiPanelEnable{ true };

	glm::vec2 currentMousePos{ 0 };
	glm::vec2 lastMousePos{ 0 };
	glm::vec2 mouseDeltaPos{ 0 };

	glm::mat4 view = glm::mat4(1.0f);

	FrameBuffer* viewportFrameBuffer;
	FrameBuffer* gameSceneFrameBuffer;

	DebugModels* debugCubesModel;
	DebugModels* debugCubesData;
	DebugModels* debugSpheres;
	DebugModels* debugLines;

	DebugCubeData cubeData;
	DebugLineData lineData;

	Shader defShader;
	Shader alphaBlendShader;
	Shader alphaCutOutShader;
	Shader skeletonAnimShader;
	UnlitColorShader solidColorShader;
	UnlitColorShader defInstanceShader;
	SkyBoxShader skyboxShader;



	Model* skyBox;

	Camera* viewportCamera;


	ApplicationWindow();
	virtual ~ApplicationWindow() = 0;

	void EngineUpdate();
	void EngineRender();
	void EngineShutDown();
	
	void InitializeWindow(int windowWidth, int windowHeight);
	void SetWindowIcon(const std::string& path);
	void SetBackgroundColor(const glm::vec3& color);

	void MainLoop();
	virtual void SetUp() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;
	virtual void OnPlayStateChanged(bool state) = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;
	virtual void KeyCallBack(GLFWwindow* window, int& key, int& scancode, int& action, int& mods) = 0;
	virtual void MouseButtonCallback(GLFWwindow* window, int& button, int& action, int& mods) = 0;

	void MouseHeldCallback(GLFWwindow* window, int& button, int& action, int& mods);
	void InputCallback(GLFWwindow* window, int& key, int& scancode, int& action, int& mods);
};


