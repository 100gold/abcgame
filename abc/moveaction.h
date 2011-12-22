#pragma once

class PlanetMove : public OwnedAction<GameObjectPlanet>
{
protected:
	void execute_impl(const Ogre::Real& progress_value);

public:
	PlanetMove(GameObjectPlanet* owner);
};

/// @brief see wiki: https://github.com/100gold/abcgame/wiki/SpaceMoveRules
class MoveAction : public OwnedAction<BasicMovableObject>
{
protected:
	const MovementParameterGroup m_moveparam;
	const Ogre::Real m_period;

	Ogre::Vector2 m_result_vector;

	void execute_impl(const Ogre::Real& progress_value);
	void finalizate(); 

	bool is_start_from_place();
	void get_valid_borders(Ogre::Real* mina, Ogre::Real* maxa, Ogre::Real* maxang);

public:
	struct PlaneParameters
	{
		Ogre::String material_name;
		Ogre::ColourValue vertex_colour;
		Ogre::Real detalization;
		Ogre::Real zindex;
	};

	MoveAction(BasicMovableObject* obj, const Ogre::Real period = 1);

	void select_result_vector(Ogre::Vector2& v);
	void create_move_plane(Ogre::ManualObject* target, const PlaneParameters& plane_params);
};