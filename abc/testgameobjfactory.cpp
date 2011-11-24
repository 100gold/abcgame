#include "import.h"
#include "all.h"
#include "alltests.h"
#include <boost/test/unit_test.hpp>

namespace
{

class TrivialObject : public BaseObject
{
	void nextturn(GameTurn& turn)
	{

	}

public:
	TrivialObject(GameSectorPtr sect, const pugi::xml_node& xml) :
		BaseObject(sect)
	{
	}
};

struct FactoryFixture
{
	TestGameObjectFixture::SectorMap m_map;
	TestGameObjectFixture* m_stor;

	FactoryFixture()
	{
		m_map.insert(TestGameObjectFixture::SectorMap::value_type(
			"testsector",
			"<document><displayname>testsector</displayname></document>"
			));
		m_stor = new TestGameObjectFixture(m_map);
	}
	~FactoryFixture()
	{
		delete m_stor;
	}
};

BOOST_AUTO_TEST_SUITE(gameobjfactory_suite)

BOOST_AUTO_TEST_CASE(gameobjfactory_test0)
{
	FactoryFixture fixture;
	ConreteGameObjectCreatorPtr ptr(new DefaultGameObjectCreator<TrivialObject>());

	GameObjectFactory::instanse().register_object("trivialobject", ptr);

	pugi::xml_document doc;
	doc.load("<object type=\"trivialobject\"><sector>testsector</sector></object>");

	BaseObject* obj = GameObjectFactory::instanse().create(doc.root().child("object"), fixture.m_stor);

	BOOST_CHECK(!obj->is_dead());

	obj->set_dead();
	delete obj;

	GameObjectFactory::instanse().unreg_object("trivialobject");
}

BOOST_AUTO_TEST_SUITE_END()

}
