#include "import.h"
#include "all.h"

#ifdef _DEBUG
#define CONFIG_NAME "ogre_d.cfg"
#define PLUGIN_CFG_NAME "plugins_d.cfg"
#define LOG_FILENAME "abc.log"
#else
#define CONFIG_NAME "ogre.cfg"
#define PLUGIN_CFG_NAME "plugins.cfg"
#define LOG_FILENAME "abc.log"
#endif

OgreBase::OgreBase()
{
	m_entity_id_seq = 0;
	m_log_manager = new Ogre::LogManager();
	m_log = m_log_manager->createLog(LOG_FILENAME, true, false);
	m_root = new Ogre::Root(PLUGIN_CFG_NAME, CONFIG_NAME);
	
	m_root->restoreConfig();

	Ogre::FileSystemArchiveFactory factory;
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("mesh",
		factory.getType(),
		"General",
		true);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("material",
		factory.getType(),
		"General",
		true);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General"); 
}

OgreBase::~OgreBase()
{
	delete m_root;
	m_log_manager->destroyLog(m_log);
	delete m_log_manager;
}

Ogre::RenderWindow* OgreBase::window()
{
	return m_window;
}

Ogre::Root* OgreBase::root()
{
	return m_root;
}

Ogre::SceneManager* OgreBase::scene_mgr()
{
	return m_scene_mgr;
}

void OgreBase::process_message()
{
	Ogre::WindowEventUtilities::messagePump();
}

void OgreBase::initialise()
{
	m_window = m_root->initialise(true);
	m_scene_mgr = m_root->createSceneManager(Ogre::ST_GENERIC);
}

Ogre::String OgreBase::get_entity_unique_id()
{
	std::stringstream s;
	s << "_entityid" << m_entity_id_seq++;
	return s.str();
}
