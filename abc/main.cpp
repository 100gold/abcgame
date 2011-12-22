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
		m_move_params.V = Ogre::Vector2(-50, 0);
		m_move_params.Vm = 300;
		m_move_params.Am = 70;
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

class MoveEveryTurn
{
	MoveController m_movecontroller;
	BasicMovableObject* m_target_obj;
	GameSectorViewPtr m_view;

public:
	MoveEveryTurn(OgreBase& ogrebase, World& world, BasicMovableObject* target, GameSectorViewPtr view) :
		m_movecontroller(ogrebase, world)
	{
		m_target_obj = target;
		m_view = view;
		World::TurnController::subscribe(*this);
	}

	void show()
	{
		m_view->set_select_action(m_movecontroller.start_control(m_target_obj, m_view));
	}

	void operator()(const GameTurn&)
	{
		show();
	}
};

static void work(OgreBase& ogre_base)
{
	try 
	{
		XmlResourceManager xml_resmgr;
		ogre_base.initialise();
		World world(xml_resmgr);
		GameSectorViewPtr view(new GameSectorView(ogre_base, ogre_base.scene_mgr()->getRootSceneNode()));;

		InputGrabber input_grabber(ogre_base.window());
		
		world.listen_input(input_grabber);
		view->listen_input(input_grabber);

		world.fetch_sector("testsector1.xml")->show(view);

		TestMovableObject* tobj = new TestMovableObject(world.fetch_sector("testsector1.xml"));
		tobj->move(Ogre::Vector2(200,200));

		MoveEveryTurn met(ogre_base, world, tobj, view);
		met.show();
		while (1)
		{
			world.process_turn();
			ogre_base.process_message();

			input_grabber.capture();
			view->render();

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