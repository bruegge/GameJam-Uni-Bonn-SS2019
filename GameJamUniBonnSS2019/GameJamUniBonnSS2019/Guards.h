#pragma once
#include "ModelObject.h"
#include "WorldMap.h"
#include "Player.h"
#include <vector>
#include "MenuGame.h"

class CGuards : public CModel
{
public:
	CGuards(CWorldMap* pWorldMap, CPlayer* pPlayer);
	~CGuards();

	void Draw(glm::mat4 mCamera);
	void Update();
	bool IsInView(glm::vec2 vPlayerPosition);
	void InitGuards(std::vector<glm::vec2> vecPositions);
	void ResetGame();
	void HearSound(glm::vec2 vPosition);
	static CGuards* GetGuards();

private:
	void SetRandomGuards(unsigned int nCount);
	bool Move(glm::vec2 vDirection, unsigned int nGuardID);
	void DrawNoiseLevel(glm::mat4 mCamera);

	struct SGuard
	{
		glm::vec2 Position;
		glm::vec2 InitialPosition;
		glm::vec2 PositionToGo;
		float Direction;
		float WalkAnimation;
		float NoiseLevel;
		float fill1;
		float fill2;
		float fill3;
	};

	GLuint m_nTextureNoiseID = 0;
	CShader* m_pShaderNoise = nullptr;
	CWorldMap* m_pWorldMap = nullptr;
	CPlayer* m_pPlayer = nullptr;
	unsigned int m_nCount = 0;
	SGuard m_aGuards[100];
	SGuard m_aInitialGuards[100];
	GLuint m_nGuardSSBO = 0;

};

