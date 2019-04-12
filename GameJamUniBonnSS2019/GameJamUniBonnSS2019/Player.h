#pragma once
#include "ModelObject.h"
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CPlayer : public CPlayerModel
{
public:
	CPlayer();
	~CPlayer();

	void Move(glm::vec2 direction);

private:
	glm::vec2 m_vPosition;
	float m_fAngle;
	GLuint m_nTextureID;
};

