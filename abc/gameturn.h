#pragma once

class GameTurn
{
protected:
	typedef std::list<ActionPtr> ActionPtrList;
	ActionPtrList m_actions;
	bool m_finished;

public:
	GameTurn();
	void do_turn(const Ogre::Real& progress_value);

	void put_action(ActionPtr act);

	bool is_finished();
};

class TurnController : public EventHiveOwner<GameTurn>
{
	Ogre::Timer m_turn_timer;
	GameTurn* m_next_turn;
	GameTurn* m_active_turn;
	
public:
	TurnController();

	bool have_active_turn();

	void process_turn();
	void start_process();
	void put_action(ActionPtr act);

	void listen_input(InputGrabber& input);
};