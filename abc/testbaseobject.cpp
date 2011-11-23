#include "import.h"
#include "all.h"
#include "alltests.h"
#include <boost/test/unit_test.hpp>

namespace
{

class TrivialObject : public BaseObject
{
	int& m_counter;

	void nextturn(const GameTurn& turn)
	{
		m_counter++;
	}
public:
	TrivialObject(GameSectorPtr sect, int& counter) :
		BaseObject(sect),
		m_counter(counter)		
	{
		sect->arrive(this);
	}
};

class EventListener
{
public:
	BaseObject* cobj;
	void operator()(BaseObject&obj)
	{
		cobj = &obj;
	}
};

struct EventsFixture
{
	int m_nextturncalled;
	EventListener m_evtlistener;
	TestGameObjectFixture::SectorMap m_map;
	TestGameObjectFixture* m_stor;

	EventsFixture() :
		m_nextturncalled(0)
	{
		m_map.insert(TestGameObjectFixture::SectorMap::value_type(
			"testsector",
			"<document><displayname>testsector</displayname></document>"
			));
		m_stor = new TestGameObjectFixture(m_map);
	}

	~EventsFixture()
	{
		delete m_stor;
	}
};

BOOST_FIXTURE_TEST_SUITE(baseobject_suite, EventsFixture)

BOOST_AUTO_TEST_CASE(baseobject_test0)
{
	BaseObject* obj1 = new TrivialObject(m_stor->fetch_sector("testsector"), m_nextturncalled);
	BaseObject* obj2 = new TrivialObject(m_stor->fetch_sector("testsector"), m_nextturncalled);

	m_nextturncalled=0;
	BaseObject::nextturn_for_all(GameTurn());
	BOOST_CHECK(m_nextturncalled == 2);

	auto event = BaseObject::subscribe(m_evtlistener);
	obj1->set_dead();
	BOOST_CHECK(m_evtlistener.cobj == obj1);

	bool found = false;
	BaseObject::for_each_object([&obj2, &found](BaseObject*obj){
		BOOST_CHECK(obj==obj2);
		if (obj==obj2)
		{
			found = true;
		}
	});
	BOOST_CHECK(found);
	obj2->set_dead();
	BaseObject::unsubscribe(event);

	delete obj1;
	delete obj2;
}

BOOST_AUTO_TEST_SUITE_END()

}
