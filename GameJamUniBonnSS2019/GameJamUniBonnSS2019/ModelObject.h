#pragma once
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "ErrorCheck.h"

class CModel
{
public:
	CModel();
	
	void Draw();
protected:
	
	struct SVertexData
	{
		glm::vec3 Position;
		glm::vec2 TextureCoordinate;
	};
	
	void GernerateQuad();

	CShader* m_pShader = nullptr;
	GLuint m_nVAO;
	GLuint m_nVBO;
	GLuint m_nEBO;
	GLuint m_nTextureID;
	glm::vec3 minValues;
	glm::vec3 maxValues;
};

class CPlayerModel : public CModel
{
public:
	CPlayerModel();
	~CPlayerModel();
	void Draw();

private:

};

class CEnvironmentModel : public CModel
{
public:
	CEnvironmentModel();
	~CEnvironmentModel();
	void Draw();
	void LoadMap(const char* fileName);
private:
	GLuint m_nMapSSBO = 0;
};


