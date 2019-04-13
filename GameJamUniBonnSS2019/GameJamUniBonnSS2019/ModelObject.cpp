#include <iostream>
#include "ModelObject.h"
#include <math.h>
#include "Texture.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>

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
