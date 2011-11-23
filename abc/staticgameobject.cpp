#include "import.h"
#include "all.h"

GameObjectSun::GameObjectSun(GameSectorPtr sector, const pugi::xml_node& cfg) :
	ViewableObject(sector)
{
	m_pos = Ogre::Vector2(0,0);
}

void GameObjectSun::nextturn(const GameTurn& turn)
{
}

void GameObjectSun::create_entity(Ogre::SceneManager* mgr)
{
	m_entity = mgr->createEntity(Ogre::SceneManager::PT_SPHERE);
}

GameObjectPlanet::GameObjectPlanet(GameSectorPtr sector, const pugi::xml_node& cfg) :
	ViewableObject(sector)
{
	pugi::xml_node geo = cfg.child("geometry");
	m_radius = geo.attribute("r").as_float();
	m_angle = geo.attribute("a").as_float();
	m_clockwise = geo.attribute("d").as_bool();
	update_position();
}

void GameObjectPlanet::nextturn(const GameTurn& turn)
{

}

void GameObjectPlanet::create_entity(Ogre::SceneManager* mgr)
{
	m_entity = mgr->createEntity(Ogre::SceneManager::PT_SPHERE);
}

void GameObjectPlanet::update_position()
{
	auto ang = m_clockwise ? -m_angle : m_angle;
	m_pos = Ogre::Vector2(m_radius*cos(ang), m_radius*sin(ang));
}
