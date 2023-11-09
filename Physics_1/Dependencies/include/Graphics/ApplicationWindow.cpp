#include "ApplicationWindow.h"

ApplicationWindow::ApplicationWindow()
{

}

ApplicationWindow::~ApplicationWindow()
{

}



void ApplicationWindow::InitializeWindow(int windowWidth, int windowHeight)
{
	if (!glfwInit())
	{
		Debugger::Print("GlfW Init failed");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	window = glfwCreateWindow(this->windowWidth, this->windowHeight, "Model Viewer", NULL, NULL);

	//User Pointer
	glfwSetWindowUserPointer(window, this);

	//SetWindowIcon(window, "Assets/Textures/Icon/Icon.png");
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->SetViewportSize(window, width, height);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->GetCursorCallback(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));
			app->MouseHeldCallback(window, button, action, mods);
			app->MouseButtonCallback(window, button, action, mods);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ApplicationWindow* app = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(window));

			app->KeyCallBack(window, key, scancode, action, mods);
		});


	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debugger::Print("Failed to initialize GLAD");
	}
	else
	{
		Debugger::Print("GLAD Initialized Successfully");
	}

	lightShader.LoadShader("res/Shader/LightShader.shader");
	Debugger::Print("Light Shader Id : ", lightShader.GetShaderId());

	defShader.LoadShader("res/Shader/Shader.shader");
	Debugger::Print("DefShader Shader Id : ", defShader.GetShaderId());

	camera.SetCameraHeight(windowHeight);
	camera.SetCameraWidth(windowWidth);

	camera.InitializeCamera();
}

void ApplicationWindow::Render()
{
	SetUp();

	CalculateCameraForward();

	lastFrameTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		//ProcessInput(window);
		MoveCameraKeyBoard(window);
		ProcessInput(window);

		renderer.Clear();

		PreRender();

		lightManager.RenderLight();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		defShader.Bind();

		//Model values
		defShader.SetUniformMat("projection", camera.GetMatrix());
		defShader.SetUniformMat("view", view);
		defShader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

		lightShader.Bind();

		lightShader.SetUniformMat("projection", camera.GetMatrix());
		lightShader.SetUniformMat("view", view);
		lightShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);

		renderer.Draw();

		PostRender();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}

void ApplicationWindow::SetWindowIcon(const std::string& path)
{
	GLFWimage images[1];

	Texture::LoadImage(path.c_str(), images[0]);

	glfwSetWindowIcon(window, 1, images);
}

void ApplicationWindow::SetBackgroundColor(const glm::vec3& color)
{
	renderer.SetBackgroundColor(color);
}

void ApplicationWindow::CalculateCameraForward()
{
	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));
	direction.y = glm::sin(glm::radians(cameraPitch));
	direction.z = glm::sin(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));

	cameraFront = glm::normalize(direction);
}

void ApplicationWindow::GetCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (stopMouseCallback) return;
	if (!mouseHeld) return;


	currentMousePos.x = xpos;
	currentMousePos.y = ypos;

	mouseDeltaPos = currentMousePos - lastMousePos;
	mouseDeltaPos.y = -mouseDeltaPos.y;

	MoveMouse();

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;

	mouseCameraMove = true;
	if (!mouseCameraMove)
		std::cout << "Mouse" << mouseCameraMove << std::endl;
}

void ApplicationWindow::MoveMouse()
{
	if (stopMouseCallback) return;
	if (!mouseCameraMove) return;
	//if (lastMousePos.x == 0 && lastMousePos.y == 0) return;


	if (cameraPitch > 89.0f)	cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)	cameraPitch = -89.0f;

	//std::cout << cameraYaw << std::endl;

	//std::cout << "Camera Yaw " << cameraYaw << std::endl;
	cameraYaw += mouseDeltaPos.x * mouseSens;
	cameraPitch += mouseDeltaPos.y * mouseSens;

	CalculateCameraForward();
}

void ApplicationWindow::MoveCameraKeyBoard(GLFWwindow* window)
{
	if (stopKeyCallback) return;

	float speed = moveSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraFront * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{

		cameraPos -= cameraFront * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::cross(cameraFront, cameraUp) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{

		cameraPos += glm::cross(cameraFront, cameraUp) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{

		cameraPos -= cameraUp * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraPos += cameraUp * speed;
	}
}

void ApplicationWindow::SetViewportSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ApplicationWindow::MouseHeldCallback(GLFWwindow* window, int& button, int& action, int& mods)
{
	if (stopMouseCallback) return;

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		mouseHeld = true;
	}
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
	{
		mouseHeld = false;
		mouseCameraMove = false;
	}
}