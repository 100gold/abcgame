#pragma once

class GameSectorView
{
public:
	typedef std::hash_map<ViewableObject*, Ogre::SceneNode*> BaseObjectMap;

protected:
	OgreBase& m_ogrebase;
	Ogre::SceneNode* m_root_node;
	BaseObjectMap m_visible_objects;
    Ogre::Camera* m_camera;
	Ogre::Viewport* m_viewport;

	void create_camera();
	void create_viewport();
public:
	GameSectorView(OgreBase& base, Ogre::SceneNode* root_node);

	void update_object(ViewableObject* obj);
	void remove_object(ViewableObject* obj);

	void render();
};
