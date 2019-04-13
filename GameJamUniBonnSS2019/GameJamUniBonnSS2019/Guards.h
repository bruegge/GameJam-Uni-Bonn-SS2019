#pragma once
#include "ModelObject.h"
#include "WorldMap.h"

class CGuards : public CModel
{
public:
	CGuards(CWorldMap* pWorldMap);
	~CGuards();

	void Draw(glm::mat4 mCamera);
	void Update();
	bool IsInView(glm::vec2 vPlayerPosition);

private:
	void SetRandomGuards(unsigned int nCount);
	bool Move(glm::vec2 vDirection, unsigned int nGuardID);
	
	struct SGuard
	{
		glm::vec2 Position;
		glm::vec2 InitialPosition;
		glm::vec2 PositionToGo;
		float Direction;
		float WalkAnimation;
	};

	CWorldMap* m_pWorldMap = nullptr;
	unsigned int m_nCount = 0;
	SGuard m_aGuards[100];
	GLuint m_nGuardSSBO = 0;

};

