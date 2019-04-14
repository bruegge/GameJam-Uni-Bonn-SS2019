#include "Cake.h"
#include "Texture.h"

CCake::CCake(CWorldMap* pWorldMap, CPlayer* pPlayer)
{
	m_pWorldMap = pWorldMap;
	m_pPlayer = pPlayer;
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowCake.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowCake.glsl");
	m_nTextureID = CTexture::LoadTexture2D("../textures/cake.bmp");

	glGenBuffers(1, &m_nCakeSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nCakeSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SCake) * 100, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

CCake::~CCake()
{
	delete m_pShader;
}

void SetCakeTexture(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D, nTextureID);
}

void CCake::Draw(glm::mat4 mCamera)
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);
	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nCakeSSBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetCakeTexture(m_pShader, m_nTextureID);

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

void CCake::InitCakes(std::vector<glm::vec2> vecPositions)
{
	m_nCount = vecPositions.size();
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (i < m_nCount) //active cake
		{
			m_aCakes[i].Available = 1.0f;
			m_aCakes[i].Position = vecPositions[i];
			m_aCakes[i].fill1 = 0.0f;
		}
		else //inactive cake
		{
			m_aCakes[i].Available = 0.0f;
			m_aCakes[i].Position = glm::vec2(0, 0);
			m_aCakes[i].fill1 = 0.0f;
		}
	}

	//apply changes to SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nCakeSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SCake) * m_nCount, m_aCakes, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void CCake::Update()
{
	glm::vec2 vecPlayerPosition = m_pPlayer->GetPosition();
	bool bHasChanged = false;
	for (unsigned int i = 0; i < m_nCount; ++i)
	{
		if (m_aCakes[i].Available == 1.0f)
		{
			if (glm::length(vecPlayerPosition - m_aCakes[i].Position) < 1.0f)
			{
				m_aCakes[i].Available = 0.0f;
				bHasChanged = true;
				m_pPlayer->AddCakeCounter();
			}
		}
	}

	if (bHasChanged)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nCakeSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SCake) * m_nCount, m_aCakes, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

unsigned int CCake::GetCountCakes()
{
	return m_nCount;
}

