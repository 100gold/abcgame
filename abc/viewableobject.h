#pragma once

class ViewableObject : public BaseObject
{
protected:
	Ogre::Real m_zindex;
	Ogre::Vector2 m_pos;
	Ogre::Entity* m_entity;
	Ogre::String m_viewable_obj_name;

	virtual void create_entity(Ogre::SceneManager* mgr) = 0;
	void refresh_view();

public:
	ViewableObject(GameSectorPtr sector);
	void attach_entity(Ogre::SceneManager* mgr, Ogre::SceneNode* root);

	const Ogre::Vector2& pos() const;
	const Ogre::Real& zindex() const;
	const Ogre::String& viewable_name() const;
};