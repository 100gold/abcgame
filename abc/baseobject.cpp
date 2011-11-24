#include "import.h"
#include "all.h"

BaseObject::ObjectList BaseObject::m_objlist;
BaseObject::EventMgr EventHiveOwner<BaseObject>::m_objevents;

BaseObject::BaseObject(GameSectorPtr sector) :
	m_current_sector(sector)
{
	m_is_dead = false;
	m_objlist.push_back(this);
}

BaseObject::~BaseObject()
{
	if (!m_is_dead)
	{
		throw std::logic_error("can't kill live object!");
	}
}

bool BaseObject::is_dead() const
{
	return m_is_dead;
}

void BaseObject::set_dead()
{
	m_current_sector->leave(this);
	m_is_dead = true;
	m_objevents.activate(*this);
	auto it = std::find(m_objlist.begin(), m_objlist.end(), this);
	if (it != m_objlist.end())
	{
		m_objlist.erase(it);
	}
}

void BaseObject::nextturn_for_all(GameTurn& turn)
{
	BOOST_FOREACH(BaseObject*x,m_objlist)
	{
		x->nextturn(turn);
	}
}

void BaseObject::move_to_sector(GameSectorPtr sector)
{
	m_current_sector->leave(this);
	m_current_sector = sector;
	m_current_sector->arrive(this);
}

GameSectorPtr BaseObject::current_sector() const
{
	return m_current_sector;
}

