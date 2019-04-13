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


