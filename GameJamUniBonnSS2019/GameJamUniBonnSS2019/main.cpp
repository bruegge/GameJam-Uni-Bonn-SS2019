#include <iostream>
#include "WindowGLFW.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include "ModelObject.h"
#include "ErrorCheck.h"
#include "Player.h"
#include "WorldMap.h"
#include "Guards.h"

CWindowGLFW* pWindow = nullptr;
CWorldMap* pEnvironment = nullptr;
CPlayer* pPlayer = nullptr;
CGuards* pGuards = nullptr;

void LoadContent()
{
	pWindow = new CWindowGLFW(1000, 800);
	CErrorCheck::GetOpenGLError(true);
	pEnvironment = new CWorldMap();
	pEnvironment->LoadMap("");
	pGuards = new CGuards(pEnvironment);
	CErrorCheck::GetOpenGLError(true);
	pPlayer = new CPlayer(pEnvironment);

	{ //GUI
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(pWindow->GetWindowID(), true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}
}

void RenderLoop()
{
	CErrorCheck::GetOpenGLError(true);
	glClearColor(0.1f, 0.1f, 0.02f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	bool bEnd = false;
	while (!bEnd)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and the depth buffer

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		bEnd = pWindow->ManageInputs();
		
		//input
		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (1 == present)
		{

			int axisCount;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
			float fSpeed = 0.03f;
			//std::cout << axisCount << std::endl;
			if (abs(axes[0]) > 0.01f || abs(axes[1]) > 0.01f)
			{
				pPlayer->Move(fSpeed * glm::vec2(axes[0], axes[1]));
			}
			if (abs(axes[4] + 1.0f) > 0.01f) 
			{ 
				pPlayer->AddScale((axes[4] + 1.0f) * 0.01f);
			}
			if (abs(axes[5] + 1.0f) > 0.01f)
			{ 
				pPlayer->AddScale(-(axes[5] + 1.0f) * 0.01f);
			}

		}
		//END input

		pGuards->Update();
		if (pGuards->IsInView(pPlayer->GetPosition()))
		{
			int nYouLoose = 0;
			nYouLoose++;
		}
		pEnvironment->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)), pPlayer->GetPosition());
		pPlayer->Draw(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y));
		pGuards->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)));
		
		
		{
			ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		pWindow->SwapBuffers();
	}
}

void DeleteContent()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	delete pWindow;
}

void main()
{
	LoadContent();
	RenderLoop();
	DeleteContent();
}