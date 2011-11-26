#include "import.h"
#include "all.h"

GameTurn::EventMgr EventHiveOwner<GameTurn>::m_objevents;

void GameTurn::do_turn(const Ogre::Real& progress_value)
{
	if (m_finished)
	{
		return;
	}
	BOOST_FOREACH(ActionPtr& act, m_actions)
	{
		act->execute(progress_value);
	}
	m_actions.remove_if([](const ActionPtr& act){
		return !act->is_active();
	});
	if (Math::moreeq(progress_value, 1))
	{
		m_finished = true;
		m_objevents.activate(*this);
	}
}

void GameTurn::put_action(ActionPtr act)
{
	m_actions.push_back(act);
}

GameTurn::GameTurn()
{
	m_finished = false;
}
