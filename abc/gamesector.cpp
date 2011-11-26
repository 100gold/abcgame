#include "import.h"
#include "all.h"

GameSector::GameSector(const std::string& config, XmlResourceManager& xmlmgr) :
	m_name(config),
	m_game_view(NULL)
{
	XmlResourcePtr xmlres = xmlmgr.load(config, XML_RESOURCE_GROUP_NAME);
	m_display_name = Localization::fetch_from_xml(xmlres->getDoc()->root(), "/document/displayname");
}

void GameSector::arrive(BaseObject* object)
{
	m_base_objects.push_back(object);
	if (NULL != m_game_view)
	{
		ViewableObject* vo = dynamic_cast<ViewableObject*>(object);
		if (NULL != vo)
		{
			m_game_view->update_object(vo);
		}
	}
}

void GameSector::leave(BaseObject* object)
{
	if (NULL != m_game_view)
	{
		ViewableObject* vo = dynamic_cast<ViewableObject*>(object);
		if (NULL != vo)
		{
			m_game_view->remove_object(vo);
		}
	}
	for(auto it = m_base_objects.begin(); it != m_base_objects.end(); ++it)
	{
		if (*it == object)
		{
			m_base_objects.erase(it);
			return;
		}
	}
}

const std::string& GameSector::name()
{
	return m_name;
}

GameSector::BaseObjectList::iterator GameSector::begin()
{
	return m_base_objects.begin();
}

GameSector::BaseObjectList::iterator GameSector::end()
{
	return m_base_objects.end();
}

void GameSector::hide(GameSectorView* view)
{
	if (m_game_view == view)
	{
		std::for_each(begin(), end(), [view](BaseObject* obj){
			ViewableObject* vo = dynamic_cast<ViewableObject*>(obj);
			if (NULL != vo)
			{
				view->remove_object(vo);
			}
		});
		m_game_view = NULL;
	}
}

void GameSector::show(GameSectorView* view)
{
	if (m_game_view == view)
	{
		return;
	}
	m_game_view = view;

	std::for_each(begin(), end(), [view](BaseObject* obj){
		ViewableObject* vo = dynamic_cast<ViewableObject*>(obj);
		if (NULL != vo)
		{
			view->update_object(vo);
		}
	});
}

void GameSector::refresh_viewobject(ViewableObject* object)
{
	if (NULL == m_game_view)
	{
		return;
	}
#ifdef _DEBUG
	auto it = std::find(m_base_objects.begin(), m_base_objects.end(), object);
	if (it == m_base_objects.end())
	{
		throw ESearchItem("viewable object", "m_base_objects on refresh_viewobject");
	}
#endif
	m_game_view->update_object(object);
}
