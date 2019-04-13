#include "Player.h"
#include "Texture.h"
#include "glm/gtx/vector_angle.hpp"

CPlayer::CPlayer(CWorldMap* pWorldMap)
{
	m_vPosition = glm::vec2(10, 10);
	m_fAngle = 3;
	std::vector<const char*> vecTextures;
	vecTextures.push_back("../textures/geilerTyp.png");
	vecTextures.push_back("../textures/geilerTyp_walk2.png");
	vecTextures.push_back("../textures/geilerTyp_walk1.png");
	vecTextures.push_back("../textures/geilerTyp_walk2.png");
	vecTextures.push_back("../textures/geilerTyp.png");
	vecTextures.push_back("../textures/geilerTyp_walk3.png");
	vecTextures.push_back("../textures/geilerTyp_walk4.png");
	vecTextures.push_back("../textures/geilerTyp_walk3.png");
	m_nTextureID = CTexture::LoadTexture2DArray(vecTextures);
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowPlayer.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowPlayer.glsl");
	m_fScale = 3.0f;
	m_fWalkAnimation = 0;
	m_pWorldMap = pWorldMap;
}

CPlayer::~CPlayer()
{
	delete m_pShader;
	glDeleteTextures(1, &m_nTextureID);
}

float GetAngle(glm::vec2 Vector1)
{
	Vector1 = glm::normalize(Vector1);

	float angle = glm::angle(Vector1, glm::vec2(1, 0));
	if (Vector1.y < 0)
	{
		angle = 2*3.1415926 - angle;
	}
	return angle;
}

void CPlayer::Move(glm::vec2 direction)
{
	m_fAngle = GetAngle(direction);
	m_fWalkAnimation += glm::length(direction)*5.0f;
	if (m_fWalkAnimation >= 8)
	{
		m_fWalkAnimation = 0;
	}
	if (m_pWorldMap->CollisionDetection(m_vPosition + direction))
	{
		m_vPosition += direction;
	}
}

glm::vec2 CPlayer::GetPosition()
{
	return m_vPosition;
}

float CPlayer::GetDirection()
{
	return m_fAngle;
}

void CPlayer::Draw(float fAspectRatio)
{
	CErrorCheck::GetOpenGLError(true);
	m_pShader->Bind();
	CErrorCheck::GetOpenGLError(true);
	glBindVertexArray(m_nVAO);
	CErrorCheck::GetOpenGLError(true);
	glDisable(GL_DEPTH_TEST);
	CErrorCheck::GetOpenGLError(true);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	glm::mat4 mViewProjectionMatrix = GetViewProjectionMatrixForPlayer(fAspectRatio);

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(m_pShader->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &(mViewProjectionMatrix[0][0]));

	GLint nUniformLocationDirection = glGetUniformLocation(m_pShader->GetID(), "direction");
	glUniform1f(nUniformLocationDirection, m_fAngle);

	GLint nUniformLocationWalkTextureStep = glGetUniformLocation(m_pShader->GetID(), "nWalkTextureStep");
	glUniform1ui(nUniformLocationWalkTextureStep, static_cast<unsigned int>(m_fWalkAnimation));


	CErrorCheck::GetOpenGLError(true);

	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(m_pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_nTextureID);
	CErrorCheck::GetOpenGLError(true);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates
	CErrorCheck::GetOpenGLError(true);

	GLint nLocationPlayerDirection = glGetUniformLocation(m_pShader->GetID(), "fDirection");
	glUniform1f(nLocationPlayerDirection, m_fAngle);
	CErrorCheck::GetOpenGLError(true);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	CErrorCheck::GetOpenGLError(true);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

glm::mat4 CPlayer::GetViewProjectionMatrixForMap(float fAspectRatio)
{
	glm::mat4 mProjectionMatrix = glm::perspective(100 * 3.1415926f / 180.0f, fAspectRatio, 0.01f, m_fScale + 2);
	glm::mat4 mViewMatrix = glm::lookAt(glm::vec3(m_vPosition.x, m_vPosition.y, m_fScale), glm::vec3(m_vPosition.x, m_vPosition.y, 0), glm::vec3(0,1,0));
	return mProjectionMatrix * mViewMatrix;
}

glm::mat4 CPlayer::GetViewProjectionMatrixForPlayer(float fAspectRatio)
{
	glm::mat4 mProjectionMatrix = glm::perspective(100 * 3.1415926f / 180.0f, fAspectRatio, 0.01f, m_fScale + 2);
	glm::mat4 mViewMatrix = glm::lookAt(glm::vec3(0, 0, m_fScale), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	return mProjectionMatrix * mViewMatrix;
}

void CPlayer::AddScale(float fScaleAdd)
{
	m_fScale += fScaleAdd;
	if (m_fScale < 2.0f)
	{
		m_fScale = 2.0f;
	}
	if (m_fScale > 100)
	{
		m_fScale = 100;
	}
}
