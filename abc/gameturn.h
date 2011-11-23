#pragma once

class GameTurn
{
	int m_number;
public:
	GameTurn();
	static GameTurn getnext(const GameTurn& turn);
	operator int();
};