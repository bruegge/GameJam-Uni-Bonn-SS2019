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

	static unsigned int GetLevelNumber();
	static EGameState GetGameState();
	static void SetGameState(EGameState eGameState);
	static void SetLevelNumber(unsigned int nLevelNumber);
};

