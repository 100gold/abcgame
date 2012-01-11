#pragma once

class ViewableObject;

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

	virtual void new_object_event(ViewableObject* obj);
	virtual void delete_object_event(ViewableObject* obj);
public:
	GameSectorView(OgreBase& base, Ogre::SceneNode* root_node);
	~GameSectorView();

	void update_object(ViewableObject* obj);
	void remove_object(ViewableObject* obj);
	Ogre::String get_object_name(ViewableObject* obj);

	void attach_to_object(ViewableObject* obj, Ogre::ManualObject* ent);
	void detach_from_object(ViewableObject* obj, Ogre::ManualObject* ent);

	void render();
};

typedef boost::shared_ptr<GameSectorView> GameSectorViewPtr;

class GameSectorMovableView : public GameSectorView, private MouseInputEvent
{
	class ScreenMoveEvent
	{
		GameSectorMovableView& m_parent;
		unsigned long m_lasttime;

	public:
		ScreenMoveEvent(GameSectorMovableView& parent) :
			m_parent(parent)
		{
			m_lasttime = 0;
		}
		void operator()(unsigned long time)
		{
			// time in milliseconds
			if (time - m_lasttime > 75)
			{
				m_parent.process_move_screen();
				m_lasttime = time;
			}
		}
	};
	friend ScreenMoveEvent;

	MouseTracer* m_mouse_tracer;
	int m_move_x;
	int m_move_y;
	bool m_subscribed;
	TimerCallbacks::EventHandle m_event_handle;
	ScreenMoveEvent m_move_event;

	virtual void new_object_event(ViewableObject* obj);
	virtual void delete_object_event(ViewableObject* obj);

	void mouse_moved(const OIS::MouseEvent &e);
	void mouse_clicked(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	void start_moving();
	void stop_moving();
	void process_move_screen();
public:
	GameSectorMovableView(OgreBase& base, Ogre::SceneNode* root_node);
	~GameSectorMovableView();

	void listen_input(InputGrabber& input_grabber);
	void set_select_action(SelectActionPtr act);
};