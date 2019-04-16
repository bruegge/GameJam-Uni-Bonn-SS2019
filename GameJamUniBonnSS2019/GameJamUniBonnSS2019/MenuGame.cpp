#include "MenuGame.h"
#include <vector>
#include "Texture.h"
#include "GameState.h"

static CMenuGame* pMenuGame = nullptr;

void CMenuGame::Init(CGuards* pGuards)
{
	pMenuGame = new CMenuGame(pGuards);
}

void CMenuGame::SetMenuState(CMenuGame::EMenuState eMenuState)
{
	m_eMenuState = eMenuState;
}

CMenuGame* CMenuGame::GetMenu()
{
	return pMenuGame;
}

CMenuGame::CMenuGame(CGuards* pGuards)
{
	m_pGuards = pGuards;
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowMenu.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowMenu.glsl");
	
	m_pShaderSelected = new CShader();
	m_pShaderSelected->CreateShaderProgram("../shaders/VS_ShowMenuSelection.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowMenuSelection.glsl");

	std::vector<const char*> vecTextureMenu;
	vecTextureMenu.push_back("../textures/MenuStart.png");
	vecTextureMenu.push_back("../textures/MenuContinueGame.png");
	vecTextureMenu.push_back("../textures/MenuCredits.png");
	vecTextureMenu.push_back("../textures/MenuOptions.png");
	vecTextureMenu.push_back("../textures/MenuNextLevel.png");
	vecTextureMenu.push_back("../textures/MenuMissionFailed.png");

	CErrorCheck::GetOpenGLError(true);
	m_nTextureID = CTexture::LoadTexture2DArray(vecTextureMenu);
	CErrorCheck::GetOpenGLError(true);

	m_nTextureSelected = CTexture::LoadTexture2D("../textures/MenuSelect.png");
}

CMenuGame::~CMenuGame()
{
	delete m_pShader;
	glDeleteTextures(1, &m_nTextureSelected);
	glDeleteTextures(1, &m_nTextureID);
}

void SetTexturesMenu(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, nTextureID);
}

void SetTexturesMenuSelection(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D, nTextureID);
}

void CMenuGame::Draw()
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTexturesMenu(m_pShader, m_nTextureID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	GLint nLocationMenuState = glGetUniformLocation(m_pShader->GetID(), "MenuState");
	glUniform1ui(nLocationMenuState, m_eMenuState);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
	glEnable(GL_DEPTH_TEST);

	DrawSelected();
}

void CMenuGame::DrawSelected()
{
	CErrorCheck::GetOpenGLError(true);
	m_pShaderSelected->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	CErrorCheck::GetOpenGLError(true);
	SetTexturesMenuSelection(m_pShaderSelected, m_nTextureSelected);
	CErrorCheck::GetOpenGLError(true);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	GLint nLocationSelectionPosition = glGetUniformLocation(m_pShaderSelected->GetID(), "SelectionPosition");
	glUniform1ui(nLocationSelectionPosition, m_nSelectedButton);

	CErrorCheck::GetOpenGLError(true);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	CErrorCheck::GetOpenGLError(true);

	glBindVertexArray(0);
	m_pShaderSelected->UnBind();
	CErrorCheck::GetOpenGLError(true);

	glEnable(GL_DEPTH_TEST);
}

void CMenuGame::Update(float fAxisDirection)
{
	if (fAxisDirection<-0.5f || fAxisDirection > 0.5f)
	{
		if (m_nAxisTimeOut == 0)
		{
			if (fAxisDirection < -0.5f)
			{
				m_nSelectedButton++;
			}
			else
			{
				m_nSelectedButton--;
			}
			if (m_nSelectedButton < 0)
			{
				m_nSelectedButton = 0;
			}
			switch (CGameState::GetGameState())
			{
			case CGameState::EGameState::InMenu:
				if (m_nSelectedButton >= GetCountButtons())
				{
					m_nSelectedButton = GetCountButtons()-1;
				}
				break;

			default:
				break;
			}

		}
		m_nAxisTimeOut++;
		if (m_nAxisTimeOut > 30)
		{
			m_nAxisTimeOut = 0;
		}
	}
	else
	{
		m_nAxisTimeOut = 0;
	}
}

unsigned int CMenuGame::GetCountButtons()
{
	switch (m_eMenuState)
	{
	case CMenuGame::StartMenu:
		return 4;
		break;
	case CMenuGame::Continue:
		return 5;
		break;
	case CMenuGame::Credits:
		return 1;
		break;
	case CMenuGame::Options:
		return 3;
		break;
	case CMenuGame::NextLevel:
		return 1;
		break;
	case CMenuGame::MissionFailed:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

void CMenuGame::SelectButton()
{
	switch (m_eMenuState)
	{
	case CMenuGame::StartMenu:
		if (m_nSelectedButton == 0) //start game
		{
			CGameState::SetGameState(CGameState::EGameState::StartGame);
			m_nSelectedButton = 0;
		}
		else if (m_nSelectedButton == 1) //options
		{
			m_eMenuState = EMenuState::Options;
			m_nSelectedButton = 0;
		}
		else if (m_nSelectedButton == 2) //credits
		{
			m_eMenuState = EMenuState::Credits;
			m_nSelectedButton = 0;
		}
		else if (m_nSelectedButton == 3) //exit game
		{
			CGameState::SetGameState(CGameState::EGameState::Exit);
		}
		break;
	case CMenuGame::Continue:
		if (m_nSelectedButton == 0) //continue game
		{
			CGameState::SetGameState(CGameState::EGameState::IsPlaying);
			m_nSelectedButton = 0;

		}
		else if (m_nSelectedButton == 1) //new Game
		{
			CGameState::SetGameState(CGameState::EGameState::StartGame);
			m_nSelectedButton = 0;

		}
		else if (m_nSelectedButton == 2) //options
		{
			m_eMenuState = EMenuState::Options;
			m_nSelectedButton = 0;

		}
		else if (m_nSelectedButton == 3) //credits
		{
			m_eMenuState = EMenuState::Credits;
			m_nSelectedButton = 0;

		}
		else if (m_nSelectedButton == 4) //exit game
		{
			CGameState::SetGameState(CGameState::EGameState::Exit);
		}
		break;
	case CMenuGame::Credits:
		if (m_nSelectedButton == 0) //continue game
		{
			m_eMenuState = EMenuState::StartMenu;
			m_nSelectedButton = 2;
		}
		break;
	case CMenuGame::Options:
		if (m_nSelectedButton == 0) //easy
		{
			m_eMenuState = EMenuState::StartMenu;
			CGameState::SetDegreeOfDifficulty(CGameState::EGameDegreeOfDifficulty::easy);
			m_nSelectedButton = 1;
		}
		if (m_nSelectedButton == 1) //mid
		{
			m_eMenuState = EMenuState::StartMenu;
			CGameState::SetDegreeOfDifficulty(CGameState::EGameDegreeOfDifficulty::mid); m_nSelectedButton = 1;
		}
		if (m_nSelectedButton == 2) //hard
		{
			m_eMenuState = EMenuState::StartMenu;
			CGameState::SetDegreeOfDifficulty(CGameState::EGameDegreeOfDifficulty::hard);
			m_nSelectedButton = 1;
		}
		break;
	case CMenuGame::NextLevel:
		if (m_nSelectedButton == 0) //next level
		{
			CGameState::SetGameState(CGameState::EGameState::IsPlaying);
			m_nSelectedButton = 0;
		}
		break;
	case CMenuGame::MissionFailed:
		if (m_nSelectedButton == 0) //try again
		{
			m_eMenuState = EMenuState::StartMenu;
			m_nSelectedButton = 0;
		}
		break;
	default:
		break;
	}
}