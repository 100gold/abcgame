#include "import.h"
#include "all.h"
#include "alltests.h"
#include <boost/test/unit_test.hpp>

namespace
{
	
class TestAction : public Action
{
	Ogre::Real m_progress_activate;

	void execute_impl(const Ogre::Real& progress_value) 
	{
		if (Math::moreeq(progress_value, m_progress_activate))
		{
			m_active = false;
		}
	}
public:
	TestAction(Ogre::Real progress_activate)
	{
		m_progress_activate = progress_activate;
	}
};

class TestGameTurn : public GameTurn
{
public:
	bool is_action_present(ActionPtr ptr)
	{
		BOOST_FOREACH(ActionPtr& act, m_actions)
		{
			if (act.get() == ptr.get())
			{
				return true;
			}
		}
		return false;
	}
};

BOOST_AUTO_TEST_SUITE(gameturn_suite)

BOOST_AUTO_TEST_CASE(gameturn_test0)
{
	TestGameTurn turn;
	ActionPtr act1(new TestAction(0.5));
	ActionPtr act2(new TestAction(1));
	turn.put_action(act1);
	turn.put_action(act2);

	BOOST_CHECK(act1->is_active());
	BOOST_CHECK(act2->is_active());

	turn.do_turn(0.3);
	BOOST_CHECK(act1->is_active());
	BOOST_CHECK(act2->is_active());

	turn.do_turn(0.7);
	BOOST_CHECK(!act1->is_active());
	BOOST_CHECK(!turn.is_action_present(act1));
	BOOST_CHECK(act2->is_active());
	BOOST_CHECK(turn.is_action_present(act2));

	turn.do_turn(1);
	BOOST_CHECK(!act1->is_active());
	BOOST_CHECK(!turn.is_action_present(act1));
	BOOST_CHECK(!act2->is_active());
	BOOST_CHECK(!turn.is_action_present(act2));
}

BOOST_AUTO_TEST_SUITE_END()

}