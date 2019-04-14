#pragma once
#include "ModelObject.h"
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "WorldMap.h"

class CPlayer : public CModel
{
public:
	CPlayer(CWorldMap* pWorldMap);
	~CPlayer();

	void Move(glm::vec2 direction);
	glm::vec2 GetPosition();
	float GetDirection();
	void Draw(float fAspectRatio);
	glm::mat4 GetViewProjectionMatrixForMap(float fAspectRatio);
	void AddScale(float fScaleAdd);

	float GetSpeed();
	void ResetSpeed();
private:
	glm::mat4 GetViewProjectionMatrixForPlayer(float fAspectRatio);
	
	CWorldMap* m_pWorldMap = nullptr;

	glm::vec2 m_vPosition;
	float m_fAngle;
	GLuint m_nTextureID;
	float m_fScale;
	float m_fWalkAnimation;
	float m_fSpeed = 0;
};

