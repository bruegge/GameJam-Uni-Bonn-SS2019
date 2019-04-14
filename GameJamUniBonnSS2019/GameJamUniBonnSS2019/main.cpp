#include <iostream>
#include "WindowGLFW.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include "ModelObject.h"
#include "ErrorCheck.h"
#include "Player.h"
#include "WorldMap.h"
#include "Cake.h"
#include "Guards.h"
#include <vector>

CWindowGLFW* pWindow = nullptr;
CWorldMap* pEnvironment = nullptr;
CPlayer* pPlayer = nullptr;
CGuards* pGuards = nullptr;
CCake* pCakes = nullptr;

void LoadContent()
{
	pWindow = new CWindowGLFW(1000, 800);
	CErrorCheck::GetOpenGLError(true);
	pEnvironment = new CWorldMap();
	pEnvironment->LoadMap("");
	pPlayer = new CPlayer(pEnvironment);
	pGuards = new CGuards(pEnvironment, pPlayer);
	pCakes = new CCake(pEnvironment, pPlayer);

	std::vector<glm::vec2> vecCakePositions;
	vecCakePositions.push_back(glm::vec2(20, 60));
	vecCakePositions.push_back(glm::vec2(70, 54));
	vecCakePositions.push_back(glm::vec2(44, 24));
	vecCakePositions.push_back(glm::vec2(11, 80));
	vecCakePositions.push_back(glm::vec2(89, 10));
	vecCakePositions.push_back(glm::vec2(46, 77));
	vecCakePositions.push_back(glm::vec2(66, 38));
	vecCakePositions.push_back(glm::vec2(94, 35));
	vecCakePositions.push_back(glm::vec2(8, 92));
	vecCakePositions.push_back(glm::vec2(55, 15));

	pCakes->InitCakes(vecCakePositions);
	pGuards->InitGuards(vecCakePositions);
	
	CErrorCheck::GetOpenGLError(true);
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
			//std::cout << axisCount << std::endl;
			if (abs(axes[0]) > 0.00001f || abs(axes[1]) > 0.00001f)
			{
				pPlayer->Move(glm::vec2(axes[0], axes[1]));
			}
			else
			{
				pPlayer->ResetSpeed();
			}

			if (abs(axes[4] + 1.0f) > 0.001f) 
			{ 
				pPlayer->AddScale((axes[4] + 1.0f) * 0.01f);
			}
			if (abs(axes[5] + 1.0f) > 0.001f)
			{ 
				pPlayer->AddScale(-(axes[5] + 1.0f) * 0.01f);
			}

		}
		//END input

		pGuards->Update();
		pCakes->Update();

		if (pGuards->IsInView(pPlayer->GetPosition()))
		{
			int nYouLoose = 0;
			nYouLoose++;
		}
		pEnvironment->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)), pPlayer->GetPosition());
		pPlayer->Draw(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y));
		pGuards->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)));
		pCakes->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)));

		
		{
			ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("%d / %d Cakes", pPlayer->GetCountCake(), pCakes->GetCountCakes());
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