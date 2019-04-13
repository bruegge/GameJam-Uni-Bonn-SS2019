#include "Guards.h"
#include <vector>
#include "Texture.h"
#include <stdlib.h>     
#include <time.h>       
#include "glm/gtx/vector_angle.hpp"

CGuards::CGuards(CWorldMap* pWorldMap)
{
	srand(time(NULL));
	m_pWorldMap = pWorldMap;
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowGuard.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowGuard.glsl");

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

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

void CGuards::SetRandomGuards(unsigned int nCount)
{
	for (unsigned int i = 0; i < nCount; ++i)
	{
		m_aGuards[i].Position.x = rand() % 100;
		m_aGuards[i].Position.y = rand() % 100;
		m_aGuards[i].Direction = static_cast<float>(rand() % 360) / 180 * 3.1415926f;
		m_aGuards[i].InitialPosition = m_aGuards[i].Position;
		m_aGuards[i].PositionToGo = glm::vec2(-1, -1);
		m_aGuards[i].WalkAnimation = 0;
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
			vNewPosition.x = rand() % 21 - 10;
			vNewPosition.y = rand() % 21 - 10;

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
		if (glm::length(vRelativePosition) < 10) //is in range
		{
			glm::vec2 vGuardViewDirection = glm::normalize(m_aGuards[i].PositionToGo - m_aGuards[i].Position);
			if (glm::angle(glm::normalize(vRelativePosition), vGuardViewDirection) < 68.0f / 180.0f * 3.1415926f) //is in view angle
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
					return true;
				}
			}
		}
	}
	return false;
}


