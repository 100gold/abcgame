#include "import.h"
#include "all.h"

#ifndef TEST_PROJECT

static void work(OgreBase& ogre_base)
{
	try 
	{
		XmlResourceManager xml_resmgr;
		ogre_base.initialise();
		InputGrabber input_grabber(ogre_base.window());
		World world(xml_resmgr);
		GameSectorView view(ogre_base, ogre_base.scene_mgr()->getRootSceneNode());
		world.listen_input(input_grabber);

		world.fetch_sector("testsector1.xml")->show(&view);

		{
			MovementParameterGroup paramgroup;
			paramgroup.V = Ogre::Vector2(0, 0);
			paramgroup.Vm = 300;
			paramgroup.Am = 180;
			paramgroup.Rm = Ogre::Math::PI/4;

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
		}

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