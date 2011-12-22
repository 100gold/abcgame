#include "import.h"
#include "all.h"

SelectActionPtr MoveController::start_control(BasicMovableObject* obj, GameSectorViewPtr view, Ogre::Real period /*= 1*/)
{
	hide();

	m_target = obj;
	m_current_action = new MoveAction(obj, period);
	m_view = view;

	m_move_plane = m_ogrebase.scene_mgr()->createManualObject();
	m_current_action->create_move_plane(m_move_plane, m_plane_params);
	m_view->attach_to_object(m_target, m_move_plane);

	class MoveControllerSelectAction : public SelectAction
	{
		MoveController& m_owner;
		void select(ViewableObject* obj, const Ogre::Vector3& point)
		{
			m_owner.select(obj, point);
		}
	public:
		MoveControllerSelectAction(MoveController& owner) :
			m_owner(owner)
		{
		}
	};
	return SelectActionPtr(new MoveControllerSelectAction(*this));
}

void MoveController::hide()
{
	if (NULL != m_move_plane)
	{
		m_view->detach_from_object(m_target, m_move_plane);
		m_ogrebase.scene_mgr()->destroyManualObject(m_move_plane->getName());
		m_move_plane = NULL;
	}
	if (NULL != m_current_action)
	{
		delete m_current_action;
		m_current_action = NULL;
	}
	m_view = GameSectorViewPtr();
	m_target = NULL;
}

void MoveController::select(ViewableObject* obj, const Ogre::Vector3& point)
{
	if (NULL == m_target)
	{
		hide();
		return;
	}
	ViewableObject* target = m_target;
	if (obj != target)
	{
		return;
	}

	if (NULL == m_current_action)
	{
		hide();
		return;
	}
	Ogre::Vector2 pos(point.x, point.y);
	m_current_action->select_result_vector(pos - obj->pos());

	m_world.put_action(ActionPtr(m_current_action));
	m_current_action = NULL;
	hide();
}

MoveController::MoveController(OgreBase& ogrebase, World& world) :
	m_ogrebase(ogrebase),
	m_world(world)
{
	m_target = NULL;
	m_current_action = NULL;
	m_move_plane = NULL;
	m_view = GameSectorViewPtr();

	m_meshname = Utl::create_hex_name(this);

	m_plane_params.material_name = "MovePlane";
	m_plane_params.detalization = 0.03f;
	m_plane_params.vertex_colour = Ogre::ColourValue(0.0f,1.0f,0.0f,0.3f);
	m_plane_params.zindex = ZINDEX_MOVEPLANE;
}
