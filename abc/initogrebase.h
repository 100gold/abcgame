#pragma once

class OgreBase
{
	Ogre::LogManager* m_log_manager;
	Ogre::Root* m_root;
	Ogre::RenderWindow* m_window;
	Ogre::SceneManager* m_scene_mgr;
	Ogre::Log* m_log;
	unsigned long long m_entity_id_seq;

public:
	OgreBase();
	~OgreBase();

	void initialise();

	Ogre::Root* root();
	Ogre::SceneManager* scene_mgr();
	Ogre::RenderWindow* window();

	void process_message();

	Ogre::String get_entity_unique_id();
};
