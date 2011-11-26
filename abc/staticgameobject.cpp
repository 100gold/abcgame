#include "import.h"
#include "all.h"

GameObjectSun::GameObjectSun(GameSectorPtr sector, const pugi::xml_node& cfg) :
	ViewableObject(sector)
{
	m_pos = Ogre::Vector2(0,0);
}

void GameObjectSun::nextturn(GameTurn& turn)
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

void GameObjectPlanet::nextturn(GameTurn& turn)
{
	class PlanetMove : public Action
	{
		GameObjectPlanet& m_owner;
		Ogre::Real m_lastprogress;

	protected:
		void execute_impl(const Ogre::Real& progress_value)
		{
			Ogre::Real progress_diff = progress_value-m_lastprogress;
			if (Ogre::Math::RealEqual(progress_diff, 0))
			{
				return;
			}

			Ogre::Radian ang_diff(progress_diff*Ogre::Math::PI/3);
			m_owner.m_angle += ang_diff;
			m_owner.update_position();
			m_lastprogress = progress_value;
		}
	public:
		PlanetMove(GameObjectPlanet& owner) :
			m_owner(owner)
		{
			m_lastprogress = 0;
		}
	};
	ActionPtr ptr(new PlanetMove(*this));
	turn.put_action(ptr);
}

void GameObjectPlanet::create_entity(Ogre::SceneManager* mgr)
{
	m_entity = mgr->createEntity(Ogre::SceneManager::PT_SPHERE);
}

void GameObjectPlanet::update_position()
{
	auto ang = m_clockwise ? -m_angle : m_angle;
	m_pos = Ogre::Vector2(m_radius*Ogre::Math::Cos(ang), m_radius*Ogre::Math::Sin(ang));
	refresh_view();
}
