#include "import.h"
#include "all.h"

#ifndef TEST_PROJECT

static void work(OgreBase& ogre_base)
{
	try 
	{
		XmlResourceManager xml_resmgr;
		ogre_base.initialise();
		World world(xml_resmgr);
		GameSectorView view(ogre_base, ogre_base.scene_mgr()->getRootSceneNode());

		world.fetch_sector("testsector1.xml")->show(&view);

		while (1)
		{
			ogre_base.process_message();

			view.render();

			if (ogre_base.window()->isClosed())
				break;
		}
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