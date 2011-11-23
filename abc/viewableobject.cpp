#include "import.h"
#include "all.h"

ViewableObject::ViewableObject(GameSectorPtr sector) :
	BaseObject(sector)
{
	m_entity = NULL;
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
