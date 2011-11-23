#pragma once

class GameObjectSun : public ViewableObject
{
protected:
	void nextturn(const GameTurn& turn);
	void create_entity(Ogre::SceneManager* mgr);

public:
	GameObjectSun(GameSectorPtr sector, const pugi::xml_node& cfg);

	DECLARE_GAMEOBJECT_NAME("sun")
};

AUTOREGISTER_GAMEOBJECT(GameObjectSun);

class GameObjectPlanet : public ViewableObject
{
	void update_position();

protected:
	Ogre::Real m_radius;
	Ogre::Real m_angle;
	bool m_clockwise;

	void nextturn(const GameTurn& turn);
	void create_entity(Ogre::SceneManager* mgr);

public:
	GameObjectPlanet(GameSectorPtr sector, const pugi::xml_node& cfg);

	DECLARE_GAMEOBJECT_NAME("planet")
};

AUTOREGISTER_GAMEOBJECT(GameObjectPlanet);
