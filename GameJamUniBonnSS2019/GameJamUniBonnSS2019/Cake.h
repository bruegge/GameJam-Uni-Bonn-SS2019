#pragma once
#include "ModelObject.h"
#include "Player.h"
#include "WorldMap.h"
#include <vector>

class CCake : public CModel
{
public:
	CCake(CWorldMap* pWorldMap, CPlayer* pPlayer);
	~CCake();

	void Draw(glm::mat4 mCamera);
	void Update();
	void InitCakes(std::vector<glm::vec2> vecPositions);
	unsigned int GetCountCakes();
	void ResetGame();

private:

	struct SCake
	{
		glm::vec2 Position;
		float Available;
		float fill1;
	};

	CShader* m_pShaderNoise = nullptr;
	CWorldMap* m_pWorldMap = nullptr;
	CPlayer* m_pPlayer = nullptr;
	unsigned int m_nCount = 0;
	SCake m_aCakes[100];
	SCake m_aInitCakes[100];
	GLuint m_nCakeSSBO = 0;

};

