#include "import.h"
#include "all.h"

#ifndef TEST_PROJECT

class TestMovableObject : public BasicMovableObject
{
public:
	TestMovableObject(GameSectorPtr sector) :
		BasicMovableObject(sector)
	{
		sector->arrive(this);
		m_move_params.V = Ogre::Vector2(100, 0);
		m_move_params.Vm = 300;
		m_move_params.Am = 30;
		m_move_params.Rm = Ogre::Math::PI/4;
	}
	void nextturn(GameTurn& turn)
	{
	}
	void create_entity(Ogre::SceneManager* mgr)
	{
		m_entity = mgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	}
};

class TestSelect : public SelectAction
{
	void select(ViewableObject* obj, const Ogre::Vector3& point)
	{
		BasicMovableObject* mobj = dynamic_cast<BasicMovableObject*>(obj);
		if (NULL != mobj)
		{
			Ogre::Vector2 pos(point.x, point.y);
			mobj->abs_move(pos);
		}
	}
};

static void work(OgreBase& ogre_base)
{
	try 
	{
		XmlResourceManager xml_resmgr;
		ogre_base.initialise();
		World world(xml_resmgr);
		GameSectorView view(ogre_base, ogre_base.scene_mgr()->getRootSceneNode());

		InputGrabber input_grabber(ogre_base.window());
		
		world.listen_input(input_grabber);
		view.listen_input(input_grabber);
		view.set_select_action(SelectActionPtr(new TestSelect()));

		world.fetch_sector("testsector1.xml")->show(&view);

		TestMovableObject* tobj = new TestMovableObject(world.fetch_sector("testsector1.xml"));
		tobj->move(Ogre::Vector2(200,200));
		/*
		{
			auto sector = world.fetch_sector("testsector1.xml");
			auto obj = *sector->begin();
			MoveAction<BaseObject> testact(obj,paramgroup);

			auto mobj = ogre_base.scene_mgr()->createManualObject();

			testact.create_move_plane(mobj, Ogre::ColourValue(0.0f,1.0f,0.0f,0.3f), 0.03f);

			Ogre::String name = "MovePlaneMesh";
			mobj->convertToMesh(name);
			Ogre::Entity* lEntity = ogre_base.scene_mgr()->createEntity("MovePlaneMesh");
			Ogre::SceneNode* lNode = ogre_base.scene_mgr()->getRootSceneNode()->createChildSceneNode();
			lNode->attachObject(lEntity);
			lNode->setPosition(0,0,ZINDEX_MOVEPLANE);
		}*/

		while (1)
		{
			world.process_turn();
			ogre_base.process_message();

			input_grabber.capture();
			view.render();

			if (ogre_base.window()->isClosed())
				break;
		}
		input_grabber.stop_input();
	} 
	catch(std::exception& e) 
	{
		std::cerr << "An exception has occurred: " << e.what() << std::endl;
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(e.what(), Ogre::LML_CRITICAL);
	}
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		OgreBase ogre_base;
		work(ogre_base);
	}
	catch(std::exception& e) 
	{
		std::cerr << "An exception has occurred: " << e.what() << std::endl;
	}
	return 0;
}

#endif