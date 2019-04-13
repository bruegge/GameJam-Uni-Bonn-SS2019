#include <iostream>
#include "WindowGLFW.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include "ModelObject.h"
#include "ErrorCheck.h"
#include "Player.h"
#include "WorldMap.h"

CWindowGLFW* pWindow = nullptr;
CWorldMap* pEnvironment = nullptr;
CPlayer* pPlayer = nullptr;

void LoadContent()
{
	pWindow = new CWindowGLFW(1000, 800);
	CErrorCheck::GetOpenGLError(true);
	pEnvironment = new CWorldMap();
	pEnvironment->LoadMap("");

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

		
		pEnvironment->Draw(pPlayer->GetViewProjectionMatrixForMap(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y)), pPlayer->GetPosition());
		pPlayer->Draw(static_cast<float>(pWindow->GetWindowSize().x) / static_cast<float>(pWindow->GetWindowSize().y));

		{
			ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

			/*ImGui::Text("Number instances: %i", pSettings->m_nInfoInstanceCount);               // Display some text (you can use a format strings too)
			ImGui::Text("ComputeTime CPU: %f ms", pSettings->m_dComputeTimeCPU);               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Enable WireFrames", &(pSettings->m_bEnableWireframe));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable face culling", &(pSettings->m_bEnableCullFace));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable QuadTree update", &(pSettings->m_bEnableQuadTreeUpdate));      // Edit bools storing our window open/close state
			ImGui::Checkbox("Enable TJunction elimination", &(pSettings->m_bEnableTJunctionElimination));      // Edit bools storing our window open/close state

			int nRadioButtonValue;
			ImGui::RadioButton("Number 1", &nRadioButtonValue, 1);
			ImGui::RadioButton("Number 2", &nRadioButtonValue, 2);

			float fGridSize = static_cast<float>(pSettings->m_nGeometrySize);
			ImGui::SliderFloat("single grid size", &fGridSize, 2.0f, 33.0f);
			pSettings->m_nGeometrySize = static_cast<unsigned int>(fGridSize);
			pSettings->m_nGeometrySize += 1 - pSettings->m_nGeometrySize % 2;


			float fMaxQuadTreeDepth = static_cast<float>(pSettings->m_nMaxQuadTreeDepth);
			ImGui::SliderFloat("QuadTree Max Depth", &fMaxQuadTreeDepth, 0.0f, 20.0f);
			pSettings->m_nMaxQuadTreeDepth = static_cast<unsigned int>(fMaxQuadTreeDepth);

			ImGui::SliderFloat("QuadTree Division Angle", &pSettings->m_fTileDivisionAngle, 90.0f, 6.0f);

			float fPerlinNoiseCount = static_cast<float>(pSettings->m_nPerlinNoiseCount);
			ImGui::SliderFloat("Number iterations perlin noise", &fPerlinNoiseCount, 0.0f, 30.0f);
			pSettings->m_nPerlinNoiseCount = static_cast<unsigned int>(fPerlinNoiseCount);

			*/
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