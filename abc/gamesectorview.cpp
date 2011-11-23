#include "import.h"
#include "all.h"

Ogre::String create_obj_name(void* obj)
{
	std::stringstream objname;
	objname << std::hex << obj;
	return objname.str();
}

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

void GameSectorView::update_object(ViewableObject* obj)
{
	BaseObjectMap::iterator it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		Ogre::SceneNode* node = m_root_node->createChildSceneNode(create_obj_name(obj));
		obj->attach_entity(m_ogrebase.scene_mgr(), node);
		m_visible_objects.insert(std::make_pair(obj,node));
		it = m_visible_objects.find(obj);
	}
	it->second->setPosition(it->first->pos().x, it->first->pos().y, 0);
}

void GameSectorView::remove_object(ViewableObject* obj)
{
	auto it = m_visible_objects.find(obj);
	if (it == m_visible_objects.end())
	{
		return;
	}

	it->second->detachAllObjects();
	it->second->removeAndDestroyAllChildren();
	m_root_node->removeAndDestroyChild(create_obj_name(obj));
	m_visible_objects.erase(it);
}

void GameSectorView::create_camera()
{
	m_camera = m_ogrebase.scene_mgr()->createCamera("PlayerCam");

	m_camera->setPosition(Ogre::Vector3(0,0,1000));
	m_camera->lookAt(Ogre::Vector3(0,0,-300));
	m_camera->setNearClipDistance(5);
}

void GameSectorView::create_viewport()
{
	// Create one viewport, entire window
	m_viewport = m_ogrebase.window()->addViewport(m_camera);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight()));
}

