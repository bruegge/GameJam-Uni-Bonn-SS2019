#pragma once
class CGameState
{
public:

	enum EGameState : unsigned int
	{
		none = 0,
		StartGame = 1,
		IsPlaying = 2,
		InMenu = 3,
		Exit = 4,
	};

	enum EGameDegreeOfDifficulty : unsigned int
	{
		easy = 0,
		mid = 1,
		hard = 2,
	};

	static unsigned int GetLevelNumber();
	static EGameState GetGameState();
	static EGameDegreeOfDifficulty GetDegreeOfDifficulty();
	static void SetDegreeOfDifficulty(EGameDegreeOfDifficulty eDegree);
	static void SetGameState(EGameState eGameState);
	static void SetLevelNumber(unsigned int nLevelNumber);
};

