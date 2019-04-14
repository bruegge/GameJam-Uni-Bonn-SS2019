#pragma once
#include "ModelObject.h"
#include "GameState.h"
#include "Guards.h"

class CGuards;

class CMenuGame : public CModel
{
public:
	enum EMenuState : unsigned int
	{
		StartMenu = 0,
		Continue = 1,
		Credits = 2,
		Options = 3,
		NextLevel = 4,
		MissionFailed = 5,
	};

	static void Init(CGuards* pGuards);
	static CMenuGame* GetMenu();
	void Draw();
	void Update(float fAxisDirection);
	void SelectButton();
	void SetMenuState(EMenuState eMenuState);

private:

	CMenuGame(CGuards* pGuards);
	~CMenuGame();

	void DrawSelected();
	unsigned int GetCountButtons();

	CGuards* m_pGuards = nullptr;
	int m_nSelectedButton = 0;
	unsigned int m_nAxisTimeOut = 0;
	CShader* m_pShaderSelected = nullptr;
	GLuint m_nTextureSelected = 0;
	EMenuState m_eMenuState = EMenuState::StartMenu;
};

