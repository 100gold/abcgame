#include "import.h"
#include "all.h"

GameSectorView::GameSectorView(OgreBase& base, Ogre::SceneNode* root_node) :
	m_ogrebase(base),
	m_root_node(root_node)
{
	create_camera();
	create_viewport();
	m_mouse_tracer = new MouseTracer(base.scene_mgr(), m_camera);
}

void GameSectorView::render()
{
	if (!m_ogrebase.root()->renderOneFrame())
	{
		throw std::logic_error("render error");
	}
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

		m_mouse_tracer->reg_obj(obj->viewable_name(), obj);
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

	m_mouse_tracer->unreg_obj(obj->viewable_name());

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
	delete m_mouse_tracer;
	m_ogrebase.window()->removeViewport(m_viewport->getZOrder());
	m_ogrebase.scene_mgr()->destroyCamera(m_camera);
}

void GameSectorView::listen_input(InputGrabber& input_grabber)
{
	input_grabber.inject_listener(m_mouse_tracer);
}

void GameSectorView::set_select_action(SelectActionPtr act)
{
	m_mouse_tracer->set_select_action(act);
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

