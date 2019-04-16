#include "GameState.h"

static CGameState::EGameState eState = CGameState::EGameState::InMenu;
static CGameState::EGameDegreeOfDifficulty eDegreeOfDifficulty = CGameState::EGameDegreeOfDifficulty::mid;
static unsigned int nLevel = 0;

unsigned int CGameState::GetLevelNumber()
{
	return nLevel;
}

CGameState::EGameState CGameState::GetGameState()
{
	return eState;
}

void CGameState::SetGameState(CGameState::EGameState eGameState)
{
	eState = eGameState;
}

void CGameState::SetLevelNumber(unsigned int nLevelNumber)
{
	nLevel = nLevelNumber;
}

CGameState::EGameDegreeOfDifficulty CGameState::GetDegreeOfDifficulty()
{
	return eDegreeOfDifficulty;
}

void CGameState::SetDegreeOfDifficulty(CGameState::EGameDegreeOfDifficulty eDegree)
{
	eDegreeOfDifficulty = eDegree;
}
