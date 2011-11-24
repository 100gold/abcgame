#include "import.h"
#include "all.h"
#include "alltests.h"
#include <boost/test/unit_test.hpp>

namespace
{

class TrivialObject : public ViewableObject
{
protected:
	void nextturn(GameTurn& turn)
	{

	}
	void create_entity(Ogre::SceneManager* mgr)
	{
		m_entity = mgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	}

public:
	TrivialObject(GameSectorPtr sect) :
		ViewableObject(sect)
	{
		  sect->arrive(this);
	}
};

class TestGameSectorView : public GameSectorView
{
public:
	TestGameSectorView(OgreBase& base, Ogre::SceneNode* root_node) :
		GameSectorView(base,root_node)
	{
	}

	bool is_object_present(ViewableObject* obj)
	{
		return m_visible_objects.find(obj) != m_visible_objects.end();
	}
};

struct BaseViewFixture
{
	TestGameObjectFixture::SectorMap m_map;
	TestGameObjectFixture* m_stor;
	TestGameSectorView* test_view;

	BaseViewFixture()
	{
		m_map.insert(TestGameObjectFixture::SectorMap::value_type(
			"testsector1",
			"<document><displayname>testsector1</displayname></document>"
			));
		m_map.insert(TestGameObjectFixture::SectorMap::value_type(
			"testsector2",
			"<document><displayname>testsector2</displayname></document>"
			));
		m_stor = new TestGameObjectFixture(m_map);
		m_stor->m_ogreroot.initialise();

		test_view = new TestGameSectorView(m_stor->m_ogreroot, m_stor->m_ogreroot.scene_mgr()->getRootSceneNode());
	}
	~BaseViewFixture()
	{
		delete test_view;
		delete m_stor;
	}
};

BOOST_FIXTURE_TEST_SUITE(baseview_suite, BaseViewFixture)

BOOST_AUTO_TEST_CASE(baseview_test0)
{
	GameSectorPtr sector1 = m_stor->fetch_sector("testsector1");
	GameSectorPtr sector2 = m_stor->fetch_sector("testsector2");
	TestObjectPtr<ViewableObject> obj1 = new TrivialObject(sector1);
	TestObjectPtr<ViewableObject> obj2 = new TrivialObject(sector1);
	TestObjectPtr<ViewableObject> obj3 = new TrivialObject(sector1);

	sector1->show(test_view);
	BOOST_CHECK(test_view->is_object_present(obj1));
	BOOST_CHECK(test_view->is_object_present(obj2));
	BOOST_CHECK(test_view->is_object_present(obj3));

	obj2->move_to_sector(sector2);
	BOOST_CHECK(test_view->is_object_present(obj1));
	BOOST_CHECK(!test_view->is_object_present(obj2));
	BOOST_CHECK(test_view->is_object_present(obj3));

	obj3->move_to_sector(sector2);
	BOOST_CHECK(test_view->is_object_present(obj1));
	BOOST_CHECK(!test_view->is_object_present(obj2));
	BOOST_CHECK(!test_view->is_object_present(obj3));

	sector2->show(test_view);
	BOOST_CHECK(test_view->is_object_present(obj1));
	BOOST_CHECK(test_view->is_object_present(obj2));
	BOOST_CHECK(test_view->is_object_present(obj3));

	sector1->hide(test_view);
	BOOST_CHECK(!test_view->is_object_present(obj1));
	BOOST_CHECK(test_view->is_object_present(obj2));
	BOOST_CHECK(test_view->is_object_present(obj3));

	obj2->move_to_sector(sector1);
	BOOST_CHECK(!test_view->is_object_present(obj1));
	BOOST_CHECK(!test_view->is_object_present(obj2));
	BOOST_CHECK(test_view->is_object_present(obj3));

	sector2->hide(test_view);
	BOOST_CHECK(!test_view->is_object_present(obj1));
	BOOST_CHECK(!test_view->is_object_present(obj2));
	BOOST_CHECK(!test_view->is_object_present(obj3));
}

BOOST_AUTO_TEST_SUITE_END()

}
