#include "import.h"
#include "all.h"

GameSectorView::GameSectorView(OgreBase& base, Ogre::SceneNode* root_node) :
	m_ogrebase(base),
	m_root_node(root_node)
{
	create_camera();
	create_viewport();
}

void GameSectorView::render()
{
	if (!m_ogrebase.root()->renderOneFrame())
	{
		throw std::logic_error("render error");
	}
}

void GameSectorView::new_object_event(ViewableObject* obj)
{
}

void GameSectorView::delete_object_event(ViewableObject* obj)
{
}

void GameSectorView::update_object(ViewableObject* obj)
{
	BaseObjectMap::iterator it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		Ogre::SceneNode* node = m_root_node->createChildSceneNode(obj->viewable_name());
		obj->attach_entity(m_ogrebase.scene_mgr(), node);
		m_visible_objects.insert(std::make_pair(obj,node));
		it = m_visible_objects.find(obj);

		new_object_event(obj);
	}
	it->second->setPosition(it->first->pos().x, it->first->pos().y,it->first->zindex());
}

void GameSectorView::remove_object(ViewableObject* obj)
{
	auto it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		return;
	}

	delete_object_event(obj);
	
	it->second->detachAllObjects();
	it->second->removeAndDestroyAllChildren();
	m_root_node->removeAndDestroyChild(obj->viewable_name());
	m_visible_objects.erase(it);
}

void GameSectorView::create_camera()
{
	m_camera = m_ogrebase.scene_mgr()->createCamera("PlayerCam");

	m_camera->setPosition(Ogre::Vector3(0,0,1000));
	m_camera->lookAt(Ogre::Vector3(0,0,-300));
	m_camera->setNearClipDistance(5);

	m_ogrebase.scene_mgr()->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
	m_ogrebase.scene_mgr()->createLight("CamLight")->setPosition(0, 0, 0);
	m_camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
}

void GameSectorView::create_viewport()
{
	// Create one viewport, entire window
	m_viewport = m_ogrebase.window()->addViewport(m_camera);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight()));
}

GameSectorView::~GameSectorView()
{
	m_ogrebase.window()->removeViewport(m_viewport->getZOrder());
	m_ogrebase.scene_mgr()->destroyCamera(m_camera);
}

void GameSectorView::attach_to_object(ViewableObject* obj, Ogre::ManualObject* ent)
{
	BaseObjectMap::iterator it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		throw ENoViewableNode("attach entity to invalid object");
	}

	it->second->attachObject(ent);
}

void GameSectorView::detach_from_object(ViewableObject* obj, Ogre::ManualObject* ent)
{
	BaseObjectMap::iterator it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		throw ENoViewableNode("detach entity from invalid object");
	}
	if (ent != it->second->detachObject(ent->getName()))
	{
		throw ENoViewableNode("detach entity, which not attached");
	}
}


GameSectorMovableView::GameSectorMovableView(OgreBase& base, Ogre::SceneNode* root_node) :
	GameSectorView(base, root_node),
	m_move_event(*this)
{
	m_mouse_tracer = new MouseTracer(base.scene_mgr(), m_camera);
	m_move_x = 0;
	m_move_y = 0;
	m_subscribed = false;
}

GameSectorMovableView::~GameSectorMovableView()
{
	delete m_mouse_tracer;
}

void GameSectorMovableView::new_object_event(ViewableObject* obj)
{
	GameSectorView::new_object_event(obj);
	m_mouse_tracer->reg_obj(obj->viewable_name(), obj);
}

void GameSectorMovableView::delete_object_event(ViewableObject* obj)
{
	m_mouse_tracer->unreg_obj(obj->viewable_name());
	GameSectorView::delete_object_event(obj);
}

void GameSectorMovableView::listen_input(InputGrabber& input_grabber)
{
	input_grabber.inject_listener(m_mouse_tracer);
	input_grabber.inject_listener(this);
}

void GameSectorMovableView::set_select_action(SelectActionPtr act)
{
	m_mouse_tracer->set_select_action(act);
}

void GameSectorMovableView::mouse_moved(const OIS::MouseEvent &e)
{
	bool need_to_move = false;
	if ((1.0*e.state.X.abs)/e.state.width < 0.03)
	{
		m_move_x = -1;
		need_to_move = true;
	}
	else if ((1.0*e.state.X.abs)/e.state.width > 0.97)
	{
		m_move_x = 1;
		need_to_move = true;
	}
	else
	{
		m_move_x = 0;
	}

	if ((1.0*e.state.Y.abs)/e.state.height < 0.03)
	{
		m_move_y = 1;
		need_to_move = true;
	}
	else if ((1.0*e.state.Y.abs)/e.state.height > 0.97)
	{
		m_move_y = -1;
		need_to_move = true;
	}
	else
	{
		m_move_y = 0;
	}

	if (need_to_move)
	{
		start_moving();
	}
	else
	{
		stop_moving();
	}
}

void GameSectorMovableView::mouse_clicked(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

}

void GameSectorMovableView::start_moving()
{
	if (!m_subscribed)
	{
		m_event_handle = TimerCallbacks::subscribe(m_move_event);
		m_subscribed = true;
	}
}

void GameSectorMovableView::stop_moving()
{
	if (m_subscribed)
	{
		TimerCallbacks::unsubscribe(m_event_handle);
		m_subscribed = false;
	}
}

void GameSectorMovableView::process_move_screen()
{
	/// 10 is size of "screen scrolling step"
	m_camera->setPosition(m_camera->getPosition() + Ogre::Vector3(m_move_x*25, m_move_y*25, 0));
}
