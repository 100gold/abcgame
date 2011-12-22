#pragma once

class MoveController
{
	OgreBase& m_ogrebase;
	World& m_world;
	Ogre::String m_meshname;
	MoveAction::PlaneParameters m_plane_params;

	BasicMovableObject* m_target;
	MoveAction* m_current_action;
	GameSectorViewPtr m_view;
	Ogre::ManualObject* m_move_plane;

	void select(ViewableObject* obj, const Ogre::Vector3& point);
public:
	MoveController(OgreBase& ogrebase, World& world);
	SelectActionPtr start_control(BasicMovableObject* obj, GameSectorViewPtr view, Ogre::Real period = 1);
	void hide();
};