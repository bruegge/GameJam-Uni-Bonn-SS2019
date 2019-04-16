#include "StoneGame.h"
#include "Texture.h"
#include "Guards.h"

static CStoneGame* pStones;

CStoneGame::CStoneGame(CPlayer* pPlayer, CWorldMap* pWorldMap)
{
	pStones = this;
	m_pPlayer = pPlayer;
	m_pWorldMap = pWorldMap;
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowStone.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowStone.glsl");
	m_nTextureID = CTexture::LoadTexture2D("../textures/stone.png");

	for (unsigned int i = 0; i < 100; ++i)
	{
		m_aStones[i].Active = 0;
		m_aStones[i].Direction = glm::vec2(0, 0);
		m_aStones[i].fill1 = 0;
		m_aStones[i].fill2 = 0;
		m_aStones[i].Position = glm::vec2(0, 0);
		m_aStones[i].Speed = 0;
	}

	glGenBuffers(1, &m_nStonesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nStonesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SStone) * 100, m_aStones, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void SetStoneTexture(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D, nTextureID);
}

CStoneGame::~CStoneGame()
{
	delete m_pShader;
}

void CStoneGame::Draw(glm::mat4 mCamera)
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nStonesSSBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetStoneTexture(m_pShader, m_nTextureID);

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
	glEnable(GL_DEPTH_TEST);
}

void CStoneGame::Update()
{
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (m_aStones[i].Active == 1)
		{
			m_aStones[i].Speed -= 0.005f;
			if (m_aStones[i].Speed + 0.005f > 0 && m_aStones[i].Speed < 0)
			{
				CGuards::GetGuards()->HearSound(m_aStones[i].Position);
			}
			if (m_aStones[i].Speed < 0.0f)
			{
				m_aStones[i].Speed = 0;
			}

			glm::vec2 position = m_aStones[i].Position + m_aStones[i].Direction * m_aStones[i].Speed;
			if(!m_pWorldMap->CollisionDetection(position)) //collision
			{
				CGuards::GetGuards()->HearSound(position);
				if (m_pWorldMap->CollisionDetection(glm::vec2(position.x, m_aStones[i].Position.y)))
				{
					m_aStones[i].Direction.y *= -1.0f;
				}
				if (m_pWorldMap->CollisionDetection(glm::vec2(m_aStones[i].Position.x, position.y)))
				{
					m_aStones[i].Direction.x *= -1.0f;
				}
			}
			else
			{
				m_aStones[i].Position = position;
			}
		}
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nStonesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SStone) * m_nCount, m_aStones, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void CStoneGame::Fire()
{
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (m_aStones[i].Active == 0)
		{
			m_aStones[i].Speed = 0.3f;
			m_aStones[i].Position = m_pPlayer->GetPosition();
			m_aStones[i].Active = 1;
			m_aStones[i].Direction = glm::vec2(cos(m_pPlayer->GetDirection()),sin(m_pPlayer->GetDirection()));
			m_aStones[i].fill1 = 0;
			m_aStones[i].fill2 = 1;
			m_nCount++;
			break;
		}
	}
}

CStoneGame* CStoneGame::GetStones()
{
	return pStones;
}

unsigned int CStoneGame::GrapStone(glm::vec2 vPosition)
{
	unsigned int nResult = 0;
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (m_aStones[i].Active)
		{
			if (glm::length(vPosition - m_aStones[i].Position) < 1.0f && m_aStones[i].Speed == 0)
			{
				nResult++;
				m_aStones[i].Active = 0;
			}
		}
	}
	return nResult;
}