#include "import.h"
#include "all.h"

void PlanetMove::execute_impl(const Ogre::Real& progress_value)
{
	Ogre::Radian ang_diff(progress_value*Ogre::Math::PI/3);
	m_owner->m_angle += ang_diff;
	m_owner->update_position();
}

PlanetMove::PlanetMove(GameObjectPlanet* owner) :
	OwnedAction(owner)
{
}

void MoveAction::execute_impl(const Ogre::Real& progress_value)
{
	m_owner->move(m_result_vector*progress_value);
}

MoveAction::MoveAction(BasicMovableObject* obj, const Ogre::Real period /*= 1*/) :
	m_moveparam(obj->move_params()),
	m_period(period),
	OwnedAction(obj),
	m_result_vector(0, 0)
{
}

bool MoveAction::is_start_from_place()	
{
	return Ogre::Math::RealEqual(m_moveparam.V.squaredLength(), 0);
}

void MoveAction::get_valid_borders(Ogre::Real* mina, Ogre::Real* maxa, Ogre::Real* maxang)
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

void MoveAction::create_move_plane(Ogre::ManualObject* target, const PlaneParameters& plane_params)
{
	Ogre::Real mina;
	Ogre::Real maxa;
	Ogre::Real maxang;
	get_valid_borders(&mina, &maxa, &maxang);

	Ogre::Real current_angle = maxang;
	Ogre::Vector2 work_vector;

	target->begin(plane_params.material_name, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
	while(1)
	{
		Ogre::Real sinr = Ogre::Math::Sin(current_angle, false);
		Ogre::Real cosr = Ogre::Math::Cos(current_angle, false);
		if (is_start_from_place())
		{
			work_vector.x = cosr;
			work_vector.y = sinr;

			target->position(maxa*cosr, maxa*sinr, plane_params.zindex);
			target->colour(plane_params.vertex_colour);

			target->position(0, 0, plane_params.zindex);
			target->colour(plane_params.vertex_colour);
		}
		else
		{
			work_vector.x = m_moveparam.V.x*cosr-m_moveparam.V.y*sinr;
			work_vector.y = m_moveparam.V.x*sinr+m_moveparam.V.y*cosr;

			target->position(work_vector.x + maxa*work_vector.normalisedCopy().x,
				work_vector.y + maxa*work_vector.normalisedCopy().y,
				plane_params.zindex);
			target->colour(plane_params.vertex_colour);

			target->position(work_vector.x + mina*work_vector.normalisedCopy().x,
				work_vector.y + mina*work_vector.normalisedCopy().y,
				plane_params.zindex);
			target->colour(plane_params.vertex_colour);
		}

		if (Ogre::Math::RealEqual(current_angle, -maxang))
		{
			break;
		}
		current_angle -= plane_params.detalization;
		if (current_angle < -maxang)
		{
			current_angle = -maxang;
		}
	}

	target->end();
}

void MoveAction::select_result_vector(Ogre::Vector2& v)
{
#pragma message("check vector and optimize")
	m_result_vector = v;
}

void MoveAction::finalizate()
{
	m_owner->set_move_vector(m_result_vector);
}
