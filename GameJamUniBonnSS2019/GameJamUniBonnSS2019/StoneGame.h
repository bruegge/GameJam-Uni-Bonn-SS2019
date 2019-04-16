#pragma once
#include "ModelObject.h"
#include "Player.h"
#include "WorldMap.h"
#include <vector>

class CStoneGame : public CModel
{
public:
	CStoneGame(CPlayer* pPlayer, CWorldMap* pWorldMap);
	~CStoneGame();

	void Draw(glm::mat4 mCamera);
	void Update();
	void Fire();
	static CStoneGame* GetStones();
	unsigned int GrapStone(glm::vec2 vPosition);


private:

	struct SStone
	{
		glm::vec2 Position;
		glm::vec2 Direction;
		float Active;
		float Speed;
		float fill1;
		float fill2;
	};

	CShader* m_pShaderNoise = nullptr;
	CPlayer* m_pPlayer = nullptr;
	CWorldMap* m_pWorldMap = nullptr;
	unsigned int m_nCount = 0;
	SStone m_aStones[100];
	GLuint m_nStonesSSBO = 0;
};

