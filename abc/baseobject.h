#pragma once

class GameSectorView;

class BaseObject : public EventHiveOwner<BaseObject>
{
	typedef std::vector<BaseObject*> ObjectList;
	static BaseObject::ObjectList m_objlist;

	bool m_is_dead;
	GameSectorPtr m_current_sector;

protected:
	virtual void nextturn(GameTurn& turn) = 0;

public:
	BaseObject(GameSectorPtr sector);
	virtual ~BaseObject();

	bool is_dead() const;
	void set_dead();

	void move_to_sector(GameSectorPtr sector);
	GameSectorPtr current_sector() const;

	template<class Callback>
	static void for_each_object(Callback& cb)
	{
		std::for_each(m_objlist.begin(), m_objlist.end(), cb);
	}
	static void nextturn_for_all(GameTurn& turn);
};
