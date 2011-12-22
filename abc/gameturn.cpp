#include "import.h"
#include "all.h"

TurnController::EventMgr EventHiveOwner<GameTurn>::m_objevents;

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

bool GameTurn::is_finished()
{
	return m_finished;
}

void TurnController::process_turn()
{
	if (NULL != m_active_turn)
	{
		bool finished_now = !m_active_turn->is_finished();
		m_active_turn->do_turn(Ogre::Real(m_turn_timer.getMilliseconds())/2000);
		if (m_active_turn->is_finished())
		{
			if (finished_now)
			{
				m_objevents.activate(*m_active_turn);
			}
			delete m_active_turn;
			m_active_turn = NULL;
		}
	}
}

TurnController::TurnController()
{
	m_active_turn = NULL;
	m_next_turn = new GameTurn();
}

void TurnController::start_process()
{
	if (NULL != m_active_turn)
	{
		throw EInconsistent("new turn before previous completed");
	}

	BaseObject::nextturn_for_all(*m_next_turn);

	m_active_turn = m_next_turn;
	m_next_turn = new GameTurn();
	m_turn_timer.reset();
}

bool TurnController::have_active_turn()
{
	return NULL != m_active_turn;
}

void TurnController::listen_input(InputGrabber& input)
{
	class NextTurn : public KeyInputEvent<KEY_NEXT_TURN>
	{
		TurnController& m_r;
		void activate()
		{
			if (!m_r.have_active_turn())
			{
				m_r.start_process();
			}
		}
	public:
		NextTurn(TurnController& r) :
			m_r(r)
		{
		}
	};
	input.inject_listener(new NextTurn(*this));
}

void TurnController::put_action(ActionPtr act)
{
	m_next_turn->put_action(act);
}
