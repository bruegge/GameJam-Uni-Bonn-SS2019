#include <iostream>
#include "ModelObject.h"
#include <math.h>
#include "Texture.h"
#include <fstream>
#include <vector>

CModel::CModel()
{
	GernerateQuad();

}

void CModel::GernerateQuad()
{
	SVertexData aQuad[4];
	
	aQuad[0].Position = glm::vec3(-0.5f, -0.5f, 0);
	aQuad[1].Position = glm::vec3(-0.5f, 0.5f, 0);
	aQuad[2].Position = glm::vec3(0.5f, 0.5f, 0);
	aQuad[3].Position = glm::vec3(0.5f, -0.5f, 0);

	aQuad[0].TextureCoordinate = glm::vec2(0, 1);
	aQuad[1].TextureCoordinate = glm::vec2(0, 0);
	aQuad[2].TextureCoordinate = glm::vec2(1, 0);
	aQuad[3].TextureCoordinate = glm::vec2(1, 1);

	GLuint aEBO[6];

	aEBO[0] = 0;
	aEBO[1] = 1;
	aEBO[2] = 2;
	aEBO[3] = 0;
	aEBO[4] = 2;
	aEBO[5] = 3;
	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);

	glGenBuffers(1, &m_nVBO);
	glGenBuffers(1, &m_nEBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SVertexData) * 4, aQuad, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, aEBO, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	glBindVertexArray(0);
}

void SetTexture(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0); 
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(),"Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, nTextureID);
}

void CModel::Draw()
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTexture(m_pShader, m_nTextureID);
		
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

CPlayerModel::CPlayerModel()
{
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowGeometry.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowGeometry.glsl");
}

CPlayerModel::~CPlayerModel()
{
	delete m_pShader;
}

void CPlayerModel::Draw()
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTexture(m_pShader, m_nTextureID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

CEnvironmentModel::CEnvironmentModel()
{
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowMap.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowMap.glsl");
	
	std::vector<const char*> vecMapTiles;
	vecMapTiles.push_back("../Textures/carpet.png");
	vecMapTiles.push_back("../Textures/stonetiles.png");
	vecMapTiles.push_back("../Textures/wall.png");

	CErrorCheck::GetOpenGLError(true);
	m_nTextureID = CTexture::LoadTexture2DArray(vecMapTiles);
	CErrorCheck::GetOpenGLError(true);
	glGenBuffers(1, &m_nMapSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nMapSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 10000, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

CEnvironmentModel::~CEnvironmentModel()
{
	delete m_pShader;

	if (m_nMapSSBO)
	{
		glDeleteBuffers(1, &m_nMapSSBO);
	}
}

void CEnvironmentModel::Draw()
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTexture(m_pShader, m_nTextureID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 10000);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

void CEnvironmentModel::LoadMap(const char* fileName)
{

	
}


