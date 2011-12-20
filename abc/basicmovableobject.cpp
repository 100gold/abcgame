#include "import.h"
#include "all.h"

BasicMovableObject::BasicMovableObject(GameSectorPtr sector) :
	ViewableObject(sector)
{
}

void BasicMovableObject::move(Ogre::Vector2& v)
{
	m_pos += v;
	refresh_view();
}

void BasicMovableObject::abs_move(Ogre::Vector2& v)
{
	m_pos = v;
	refresh_view();
}
