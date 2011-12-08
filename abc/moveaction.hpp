#pragma once

class PlanetMove : public OwnedAction<GameObjectPlanet>
{
protected:
	void execute_impl(const Ogre::Real& progress_value)
	{
		Ogre::Radian ang_diff(progress_value*Ogre::Math::PI/3);
		m_owner->m_angle += ang_diff;
		m_owner->update_position();
	}
public:
	PlanetMove(GameObjectPlanet* owner) :
		OwnedAction(owner)
	{
	}
};

/// @brief see wiki: https://github.com/100gold/abcgame/wiki/SpaceMoveRules
template<class FiniteOwner>
class MoveAction : public OwnedAction<FiniteOwner>
{
protected:
	MovementParameterGroup m_moveparam;
	Ogre::Real m_a;
	Ogre::Real m_r;

	Ogre::Real m_v_module;
	Ogre::Vector2 m_r1;
	bool m_is_zero_speed;

	Ogre::Vector2 m_result_vector;

	void execute_impl(const Ogre::Real& progress_value)
	{
		m_owner->move(m_result_vector*progress_value);
	}

public:
	MoveAction(FiniteOwner* obj, const MovementParameterGroup& moveparam) :
		m_moveparam(moveparam),
		OwnedAction(obj),
		m_result_vector(0, 0)
	{
		m_v_module = m_moveparam.V.length();
		m_is_zero_speed = Ogre::Math::RealEqual(m_v_module, 0);
		if (m_is_zero_speed)
		{
			m_r1 = Ogre::Vector2(1,0);
		}
		else
		{
			m_r1 = Ogre::Vector2(
				-m_moveparam.V.y / m_moveparam.V.squaredLength(),
				m_moveparam.V.x / m_moveparam.V.squaredLength()
				);
		}
	}

	bool select_acceleration(const Ogre::Real& a, const Ogre::Real& r)
	{
		if (m_is_zero_speed)
		{
			return false;
		}
		m_a = a;
		m_r = b;
		return true;
	}

	bool select_move_vector(const Ogre::Vector2& e1)
	{
		if (!m_is_zero_speed)
		{
			return false;
		}
		m_r1 = e1;
		if (!Ogre::Math::RealEqual(m_r1.squaredLength(), 1))
		{
			m_r1.normalise();
		}
		return true;
	}

	Ogre::Vector2 calc_speed_vector(const Ogre::Real& turn_progress)
	{
		Ogre::Vector2 result;
		if (m_is_zero_speed)
		{
			result = m_r1*r*turn_progress;
		}
		else
		{
			result = (1 + m_a*turn_progress/m_v_module)*m_moveparam.V+m_r1*r*turn_progress;
		}
	}
};