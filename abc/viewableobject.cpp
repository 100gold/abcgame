#include "import.h"
#include "all.h"

ViewableObject::ViewableObject(GameSectorPtr sector) :
	BaseObject(sector)
{
	m_entity = NULL;
	m_pos = Ogre::Vector2(0,0);
	m_zindex = ZINDEX_DEFAULT;

	std::stringstream objname;
	objname << std::hex << this;
	m_viewable_obj_name = objname.str();
}

void ViewableObject::attach_entity(Ogre::SceneManager* mgr, Ogre::SceneNode* root)
{
	if (NULL == m_entity)
	{
		create_entity(mgr);
	}
	root->attachObject(m_entity);
}

const Ogre::Vector2& ViewableObject::pos() const
{
	return m_pos;
}

void ViewableObject::refresh_view()
{
	m_current_sector->refresh_viewobject(this);
}

const Ogre::Real& ViewableObject::zindex() const
{
	return m_zindex;
}

const Ogre::String& ViewableObject::viewable_name() const
{
	return m_viewable_obj_name;
}
