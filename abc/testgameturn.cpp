#include "import.h"
#include "all.h"
#include "alltests.h"
#include <boost/test/unit_test.hpp>

namespace
{

class TrivialObject : public BaseObject
{
protected:
	void nextturn(GameTurn& turn)
	{

	}

public:
	TrivialObject(GameSectorPtr sect) :
		BaseObject(sect)
	{
		  sect->arrive(this);
	}
};

class TestAction : public OwnedAction<TrivialObject>
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
	TestAction(Ogre::Real progress_activate, TrivialObject* obj) :
		OwnedAction(obj)
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

struct GameTurnFixture
{
	TestGameObjectFixture::SectorMap m_map;
	TestGameObjectFixture* m_stor;

	GameTurnFixture()
	{
		m_map.insert(TestGameObjectFixture::SectorMap::value_type(
			"testsector",
			"<document><displayname>testsector</displayname></document>"
			));
		m_stor = new TestGameObjectFixture(m_map);
	}
	~GameTurnFixture()
	{
		delete m_stor;
	}
};

BOOST_FIXTURE_TEST_SUITE(gameturn_suite, GameTurnFixture)

BOOST_AUTO_TEST_CASE(gameturn_test0)
{
	TestGameTurn turn;
	GameSectorPtr sector = m_stor->fetch_sector("testsector");
	TestObjectPtr<TrivialObject> owner1 = new TrivialObject(sector);
	ActionPtr act1(new TestAction(0.5, owner1));
	TestObjectPtr<TrivialObject> owner2 = new TrivialObject(sector);
	ActionPtr act2(new TestAction(1,owner2));

	BOOST_CHECK(act1->is_owned_by(owner1));
	BOOST_CHECK(!act1->is_owned_by(owner2));
	BOOST_CHECK(act2->is_owned_by(owner2));
	BOOST_CHECK(!act2->is_owned_by(owner1));

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