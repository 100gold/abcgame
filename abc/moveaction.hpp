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
	const MovementParameterGroup m_moveparam;
	const Ogre::Real m_period;

	Ogre::Vector2 m_result_vector;

	void execute_impl(const Ogre::Real& progress_value)
	{
		//m_owner->move(m_result_vector*progress_value);
	}

public:
	MoveAction(FiniteOwner* obj, const MovementParameterGroup& moveparam, const Ogre::Real period = 1) :
		m_moveparam(moveparam),
		m_period(period),
		OwnedAction(obj),
		m_result_vector(0, 0)
	{
	}

	bool is_start_from_place()
	{
		return Ogre::Math::RealEqual(m_moveparam.V.squaredLength(), 0);
	}

	void get_valid_borders(Ogre::Real* mina, Ogre::Real* maxa, Ogre::Real* maxang)
	{
		BOOST_ASSERT(mina!=NULL);
		BOOST_ASSERT(maxa!=NULL);
		BOOST_ASSERT(maxang!=NULL);
		if (is_start_from_place())
		{
			*maxang = Ogre::Math::PI;
			*mina = 0;
			*maxa = std::min(m_moveparam.Am*m_period, m_moveparam.Vm);
		}
		else
		{
			*maxang = m_moveparam.Rm*m_period;
			*mina = std::max(-m_moveparam.Am*m_period, -m_moveparam.V.length());
			*maxa = std::min(m_moveparam.Am*m_period, m_moveparam.Vm - m_moveparam.V.length());
		}
	}

	void create_move_plane(Ogre::ManualObject* target, const Ogre::ColourValue& vertex_colour, const Ogre::Real& detalization)
	{
		Ogre::Real mina;
		Ogre::Real maxa;
		Ogre::Real maxang;
		get_valid_borders(&mina, &maxa, &maxang);

		Ogre::Real current_angle = maxang;
		Ogre::Vector2 work_vector;

		target->begin("MovePlane", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
		while(1)
		{
			Ogre::Real sinr = Ogre::Math::Sin(current_angle, false);
			Ogre::Real cosr = Ogre::Math::Cos(current_angle, false);
			if (is_start_from_place())
			{
				work_vector.x = cosr;
				work_vector.y = sinr;

				target->position(maxa*cosr, maxa*sinr, 0);
				target->colour(vertex_colour);

				target->position(0,0,0);
				target->colour(vertex_colour);
			}
			else
			{
				work_vector.x = m_moveparam.V.x*cosr-m_moveparam.V.y*sinr;
				work_vector.y = m_moveparam.V.x*sinr+m_moveparam.V.y*cosr;

				target->position(work_vector.x + maxa*work_vector.normalisedCopy().x,
					work_vector.y + maxa*work_vector.normalisedCopy().y,
					0);
				target->colour(vertex_colour);

				target->position(work_vector.x + mina*work_vector.normalisedCopy().x,
					work_vector.y + mina*work_vector.normalisedCopy().y,
					0);
				target->colour(vertex_colour);
			}

			if (Ogre::Math::RealEqual(current_angle, -maxang))
			{
				break;
			}
			current_angle -= detalization;
			if (current_angle < -maxang)
			{
				current_angle = -maxang;
			}
		}

		target->end();
	}
};