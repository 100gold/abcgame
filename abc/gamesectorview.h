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
	MouseTracer* m_mouse_tracer;

	void create_camera();
	void create_viewport();
public:
	GameSectorView(OgreBase& base, Ogre::SceneNode* root_node);
	~GameSectorView();

	void update_object(ViewableObject* obj);
	void remove_object(ViewableObject* obj);
	Ogre::String get_object_name(ViewableObject* obj);

	void render();

	void listen_input(InputGrabber& input_grabber);
	void set_select_action(SelectActionPtr act);
};
