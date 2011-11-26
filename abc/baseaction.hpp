#pragma once

class Action
{
protected:
	bool m_active;

	virtual void execute_impl(const Ogre::Real& progress_value) = 0;

public:
	Action()
	{
		m_active = true;
	}

	void execute(const Ogre::Real& progress_value)
	{
		if (m_active)
		{
			execute_impl(progress_value);
			if (Math::moreeq(progress_value,1))
			{
				m_active = false;
			}
		}
	}

	bool is_active() const
	{
		return m_active;
	}
};

typedef boost::shared_ptr<Action> ActionPtr;