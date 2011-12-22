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

const MovementParameterGroup& BasicMovableObject::move_params()
{
	return m_move_params;
}

void BasicMovableObject::set_move_vector(Ogre::Vector2& v)
{
	m_move_params.V = v;
}
