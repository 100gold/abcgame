#include "import.h"
#include "all.h"

void MouseTracer::mouse_moved(const OIS::MouseEvent &e)
{

}

void MouseTracer::mouse_clicked(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if (id != OIS::MB_Left)
	{
		return;
	}

	Ogre::Real rx = e.state.X.abs;
	Ogre::Real ry = e.state.Y.abs;
	Ogre::Ray ray = m_camera->getCameraToViewportRay(rx/e.state.width, ry/e.state.height);
	m_ray_query->setRay(ray);
	auto result = m_ray_query->execute();

	for (auto it = result.begin(); it != result.end(); ++it)
	{
		if (NULL == it->movable)
		{
			continue;
		}

		auto event = m_registred_events.find(it->movable->getName());
		if (event == m_registred_events.end())
		{
			Ogre::Node* node = it->movable->getParentNode();
			while (NULL != node)
			{
				event = m_registred_events.find(node->getName());
				if (event != m_registred_events.end())
				{
					break;
				}
				node = node->getParent();
			}
		}

		if (event != m_registred_events.end())
		{
			m_select_action->select(event->second, ray.getOrigin() + ray.getDirection().normalisedCopy()*it->distance);
			break;
		}
	}
}

MouseTracer::MouseTracer(Ogre::SceneManager* sc_mgr, Ogre::Camera* camera)
{
	m_camera = camera;
	m_ray_query = sc_mgr->createRayQuery(Ogre::Ray());

	SelectActionPtr ptr(new SelectAction());
	m_select_action = ptr;
}

void MouseTracer::reg_obj(Ogre::String objname, ViewableObject* obj)
{
	m_registred_events.insert(std::make_pair(objname, obj));
}

void MouseTracer::unreg_obj(Ogre::String objname)
{
	auto it = m_registred_events.find(objname);
	if (it != m_registred_events.end())
	{
		m_registred_events.erase(it);
	}
}

void MouseTracer::set_select_action(SelectActionPtr act)
{
	m_select_action = act;
}

MouseTracer::~MouseTracer()
{
	delete m_ray_query;
}
