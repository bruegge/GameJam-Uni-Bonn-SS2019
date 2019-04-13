#pragma once
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "ErrorCheck.h"
#include "ModelObject.h"

class CWorldMap : public CModel
{
public:
	CWorldMap();
	~CWorldMap();

	void Draw(glm::mat4 mCamera, glm::vec2 vPlayerPosition);
	void LoadMap(const char* fileName);
	bool CollisionDetection(glm::vec2 vPosition);
private:
	GLuint m_nMapSSBO = 0;
	unsigned int m_aMap[10000];
};

