#include "import.h"
#include "all.h"

GameTurn::GameTurn()
{
	m_number = 1;
}

GameTurn GameTurn::getnext(const GameTurn& turn)
{
	GameTurn result;
	result.m_number = turn.m_number+1;
	return result;
}

GameTurn::operator int()
{
	return m_number;
}