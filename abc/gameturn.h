#pragma once

class GameTurn : public EventHiveOwner<GameTurn>
{
protected:
	typedef std::list<ActionPtr> ActionPtrList;
	ActionPtrList m_actions;
	bool m_finished;

public:
	GameTurn();
	void do_turn(const Ogre::Real& progress_value);

	void put_action(ActionPtr act);
};

class TurnController
{
	Ogre::Timer m_turn_timer;
	GameTurn* m_active_turn;
public:
	TurnController();
	void process_turn();
	void new_turn();
	bool have_active_turn();
};