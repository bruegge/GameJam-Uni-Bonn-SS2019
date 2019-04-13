#include "WorldMap.h"
#include <iostream>
#include <math.h>
#include "Texture.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>

CWorldMap::CWorldMap()
{
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowMap.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowMap.glsl");

	std::vector<const char*> vecMapTiles;
	vecMapTiles.push_back("../Textures/carpet.png");
	vecMapTiles.push_back("../Textures/stonetile2.png");
	vecMapTiles.push_back("../Textures/wall.png");

	CErrorCheck::GetOpenGLError(true);
	m_nTextureID = CTexture::LoadTexture2DArray(vecMapTiles);
	CErrorCheck::GetOpenGLError(true);
	glGenBuffers(1, &m_nMapSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nMapSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 10000, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	

}

CWorldMap::~CWorldMap()
{
	delete m_pShader;

	if (m_nMapSSBO)
	{
		glDeleteBuffers(1, &m_nMapSSBO);
	}
}

void SetTexture(CShader* pShader, GLuint nTextureID)
{
	glActiveTexture(GL_TEXTURE0);
	GLuint uniformLocation = glGetUniformLocation(pShader->GetID(), "Texture");
	glUniform1i(uniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, nTextureID);
}

void CWorldMap::Draw(glm::mat4 mCamera, glm::vec2 vPlayerPosition)
{
	m_pShader->Bind();
	glBindVertexArray(m_nVAO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nMapSSBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);

	SetTexture(m_pShader, m_nTextureID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(0 * sizeof(float))); //Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (GLvoid*)(3 * sizeof(float))); //TextureCoordinates

	GLint nUniformLocationViewProjectionMatrix = glGetUniformLocation(m_pShader->GetID(), "viewProjectionMatrix");
	glUniformMatrix4fv(nUniformLocationViewProjectionMatrix, 1, GL_FALSE, &(mCamera[0][0]));

	GLint nUniformLocationPlayerPosition = glGetUniformLocation(m_pShader->GetID(), "playerPosition");
	glUniform2f(nUniformLocationPlayerPosition, vPlayerPosition.x, vPlayerPosition.y);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 10000);

	glBindVertexArray(0);
	m_pShader->UnBind();
	CErrorCheck::GetOpenGLError(true);
}

void CWorldMap::LoadMap(const char* fileName)
{
	std::string line;
	std::ifstream myfile("../maps/Map001.map");
	if (myfile.is_open())
	{
		for (unsigned int i = 0; i < 10000; ++i)
		{
			m_aMap[i] = 0;
		}
		while (getline(myfile, line))
		{
			for (unsigned int i = 0; i < 10000; ++i)
			{
				m_aMap[i] = line.c_str()[i];
			}
		}
		myfile.close();

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_nMapSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 10000, m_aMap, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	}
}

bool CWorldMap::CollisionDetection(glm::vec2 vPosition)
{
	vPosition += glm::vec2(0.5f, 0.5f);
	if (vPosition.x >= 0 && vPosition.x <= 100 && vPosition.y >= 0 && vPosition.y <= 100)
	{
		unsigned int nIndex = static_cast<unsigned int>(vPosition.x) + 100 * static_cast<unsigned int>(vPosition.y);

		if (nIndex >= 0 && nIndex <= 10000 && m_aMap[nIndex] != 0)
		{
			return false;
		}
		
		return true;
	}
	return false;
}
