#include "Guards.h"
#include <vector>
#include "Texture.h"
#include <stdlib.h>     
#include <time.h>       
#include "glm/gtx/vector_angle.hpp"
#include "GameState.h"

static CGuards* pGuards;

CGuards::CGuards(CWorldMap* pWorldMap, CPlayer* pPlayer)
{
	pGuards = this;
	srand(static_cast<unsigned int>(time(NULL)));
	m_pWorldMap = pWorldMap;
	m_pPlayer = pPlayer;
	m_pShader = new CShader();
	m_pShaderNoise = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowGuard.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowGuard.glsl");
	m_pShaderNoise->CreateShaderProgram("../shaders/VS_ShowGuardNoiseBar.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowGuardNoiseBar.glsl");

	std::vector<const char*> vecTextureGuards;
	vecTextureGuards.push_back("../textures/geilerTyp.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk2.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk1.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk2.png");
	vecTextureGuards.push_back("../textures/geilerTyp.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk3.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk4.png");
	vecTextureGuards.push_back("../textures/geilerTyp_walk3.png");

	CErrorCheck::GetOpenGLError(true);
	m_nTextureID = CTexture::LoadTexture2DArray(vecTextureGuards);
	CErrorCheck::GetOpenGLError(true);
	
	std::vector<const char*> vecTextureNoise;
	vecTextureNoise.push_back("../textures/NoiseBarGreen.bmp");
	vecTextureNoise.push_back("../textures/NoiseBarRed.bmp");

	CErrorCheck::GetOpenGLError(true);
	m_nTextureNoiseID = CTexture::LoadTexture2DArray(vecTextureNoise);
	CErrorCheck::GetOpenGLError(true);
	
	glGenBuffers(1, &m_nGuardSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nGuardSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SGuard) * 100, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	SetRandomGuards(10);
}

CGuards::~CGuards()
{
}

void SetTextures(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, nTextureID);
}

void CGuards::Draw(glm::mat4 mCamera)
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nGuardSSBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTextures(m_pShader, m_nTextureID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(m_pShader->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &(mCamera[0][0]));

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_nCount);

	
	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);

	DrawNoiseLevel(mCamera);
	glEnable(GL_DEPTH_TEST);
}

void CGuards::DrawNoiseLevel(glm::mat4 mCamera)
{
	m_pShaderNoise->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nGuardSSBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTextures(m_pShaderNoise, m_nTextureNoiseID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(m_pShaderNoise->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &(mCamera[0][0]));

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_nCount);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
	m_pShaderNoise->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

void CGuards::SetRandomGuards(unsigned int nCount)
{
	for (unsigned int i = 0; i < nCount; ++i)
	{
		m_aGuards[i].Position.x = static_cast<float>(rand() % 100);
		m_aGuards[i].Position.y = static_cast<float>(rand() % 100);
		m_aGuards[i].Direction = static_cast<float>(rand() % 360) / 180 * 3.1415926f;
		m_aGuards[i].InitialPosition = m_aGuards[i].Position;
		m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
		m_aGuards[i].WalkAnimation = 0;
		m_aGuards[i].NoiseLevel = 0.0f;
	}
	m_nCount = nCount;
}

void CGuards::Update()
{
	//Guard movements
	for (unsigned int i = 0; i < m_nCount; ++i)
	{
		if (m_aGuards[i].PositionToGo.x == -1 && m_aGuards[i].PositionToGo.y == -1) //point reached
		{
			glm::vec2 vNewPosition;
			vNewPosition.x = static_cast<float>(rand() % 21 - 10);
			vNewPosition.y = static_cast<float>(rand() % 21 - 10);

			m_aGuards[i].PositionToGo = glm::vec2(m_aGuards[i].InitialPosition.x + vNewPosition.x, m_aGuards[i].InitialPosition.y + vNewPosition.y);
		
			if (m_aGuards[i].PositionToGo.x < 0 || m_aGuards[i].PositionToGo.x > 100 || m_aGuards[i].PositionToGo.y < 0 || m_aGuards[i].PositionToGo.y > 100)
			{
				m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
			}
		}

		bool bCanMove = Move(m_aGuards[i].PositionToGo - m_aGuards[i].Position, i);

		if (!bCanMove)
		{
			m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
		}

		if (static_cast<unsigned int>(m_aGuards[i].Position.x + 2) > static_cast<unsigned int>(m_aGuards[i].PositionToGo.x) && static_cast<unsigned int>(m_aGuards[i].Position.x - 2) < static_cast<unsigned int>(m_aGuards[i].PositionToGo.x))
		{
			if (static_cast<unsigned int>(m_aGuards[i].Position.y + 2) > static_cast<unsigned int>(m_aGuards[i].PositionToGo.y) && static_cast<unsigned int>(m_aGuards[i].Position.y - 2) < static_cast<unsigned int>(m_aGuards[i].PositionToGo.y))
			{
				m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
			}
		}

		float fDistanceToPlayer = glm::length(m_aGuards[i].Position - m_pPlayer->GetPosition());
		if (fDistanceToPlayer > 10)
		{
			m_aGuards[i].NoiseLevel = 0;
		}
		else
		{
			m_aGuards[i].NoiseLevel = (10 - fDistanceToPlayer) * 0.1f * m_pPlayer->GetSpeed() * 4.0f;
			if (m_aGuards[i].NoiseLevel >= 1.0f)
			{
				m_aGuards[i].PositionToGo = m_pPlayer->GetPosition();
			}
		}
	}

	//apply changes to SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nGuardSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SGuard) * m_nCount, m_aGuards, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

float GetAngle2D(glm::vec2 Vector1)
{
	Vector1 = glm::normalize(Vector1);

	float angle = glm::angle(Vector1, glm::vec2(1, 0));
	if (Vector1.y < 0)
	{
		angle = 2.0f * 3.1415926f - angle;
	}
	return angle;
}

bool CGuards::Move(glm::vec2 vDirection, unsigned int nGuardID)
{
	vDirection = glm::normalize(vDirection)*0.02f;
	m_aGuards[nGuardID].Direction = GetAngle2D(glm::normalize(vDirection));
	m_aGuards[nGuardID].WalkAnimation += 0.1f;
	if (m_aGuards[nGuardID].WalkAnimation >= 8)
	{
		m_aGuards[nGuardID].WalkAnimation = 0;
	}
	if (m_pWorldMap->CollisionDetection(m_aGuards[nGuardID].Position + vDirection))
	{
		m_aGuards[nGuardID].Position += vDirection;
		return true;
	}
	return false;
}

bool CGuards::IsInView(glm::vec2 vPlayerPosition)
{
	for (unsigned int i = 0; i < m_nCount; ++i)
	{
		glm::vec2 vRelativePosition = m_aGuards[i].Position - vPlayerPosition;

		CGameState::EGameDegreeOfDifficulty eDegree = CGameState::GetDegreeOfDifficulty();
		float fRange = 0;
		float fAngle = 0;
		switch (eDegree)
		{
		case CGameState::easy:
			fRange = 5;
			fAngle = 30.0f;
			break;
		case CGameState::mid:
			fRange = 7;
			fAngle = 45.0f;
			break;
		case CGameState::hard:
			fRange = 10;
			fAngle = 68.0f;
			break;
		default:
			break;
		}

		if (glm::length(vRelativePosition) < fRange) //is in range
		{
			glm::vec2 vGuardViewDirection = glm::normalize(m_aGuards[i].PositionToGo - m_aGuards[i].Position);
			if (glm::angle(glm::normalize(vRelativePosition), -vGuardViewDirection) < fAngle / 180.0f * 3.1415926f) //is in view angle
			{
				bool bNoWallInBetween = true;
				float nCountSteps = 30;
				glm::vec2 vDirection = (vPlayerPosition - m_aGuards[i].Position) / nCountSteps;
				for (unsigned int j = 0; j < nCountSteps; ++j)
				{
					if (!m_pWorldMap->CollisionDetection(m_aGuards[i].Position + static_cast<float>(j) * vDirection))
					{
						bNoWallInBetween = false;
						break;
					}
				}
				if (bNoWallInBetween)
				{
					CGameState::SetGameState(CGameState::EGameState::InMenu);
					CMenuGame::GetMenu()->SetMenuState(CMenuGame::EMenuState::MissionFailed);
					return true;
				}
			}
		}
	}
	return false;
}

void CGuards::InitGuards(std::vector<glm::vec2> vecPositions)
{
	m_nCount = vecPositions.size();
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (i < m_nCount) //active cake
		{
			m_aGuards[i].Position = vecPositions[i];
			m_aGuards[i].Direction = static_cast<float>(rand() % 360) / 180 * 3.1415926f;
			m_aGuards[i].InitialPosition = m_aGuards[i].Position;
			m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
			m_aGuards[i].WalkAnimation = 0;
			m_aGuards[i].NoiseLevel = 0.0f;

		}
		else //inactive cake
		{
			m_aGuards[i].Position = glm::vec2(-1, -1);
			m_aGuards[i].Direction = static_cast<float>(rand() % 360) / 180 * 3.1415926f;
			m_aGuards[i].InitialPosition = m_aGuards[i].Position;
			m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
			m_aGuards[i].WalkAnimation = 0;
			m_aGuards[i].NoiseLevel = 0.0f;
		}
		m_aInitialGuards[i] = m_aGuards[i];
	}

	//apply changes to SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nGuardSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SGuard) * m_nCount, m_aGuards, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void CGuards::ResetGame()
{
	for (unsigned int i = 0; i < 100; ++i)
	{
		m_aGuards[i] = m_aInitialGuards[i];
	}
}

void CGuards::HearSound(glm::vec2 vPosition)
{
	for (unsigned int i = 0; i < m_nCount; ++i)
	{
		if (glm::length(vPosition - m_aGuards[i].Position) < 10)
		{
			m_aGuards[i].PositionToGo = vPosition;
		}
	}
}

CGuards* CGuards::GetGuards()
{
	return pGuards;
}