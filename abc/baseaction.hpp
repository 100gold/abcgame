#pragma once

class BaseObject;

class Action
{
protected:
	bool m_active;
	Ogre::Real m_lastprogress;

	virtual void execute_impl(const Ogre::Real& progress_value) = 0;
public:
	Action()
	{
		m_active = true;
		m_lastprogress = 0;
	}

	void execute(const Ogre::Real& progress_value)
	{
		if (!m_active)
		{
			return;
		}
		
		Ogre::Real progress_diff = progress_value-m_lastprogress;
		if (Ogre::Math::RealEqual(progress_diff, 0))
		{
			return;
		}

		execute_impl(progress_diff);
		m_lastprogress = progress_value;
		if (Math::moreeq(progress_value, 1))
		{
			m_active = false;
		}
	}

	bool is_active() const
	{
		return m_active;
	}

	virtual bool is_owned_by(BaseObject* obj) = 0;
};

template<class FiniteOwner>
class OwnedAction : public Action
{
	BaseObject* m_basicowner;
protected:
	FiniteOwner* m_owner;

public:
	OwnedAction(FiniteOwner* owner) :
		m_owner(owner),
		m_basicowner(dynamic_cast<BaseObject*>(owner)),
		Action()
	{
	}

	bool is_owned_by(BaseObject* obj)
	{
		return m_basicowner == obj;
	}
};

typedef boost::shared_ptr<Action> ActionPtr;
