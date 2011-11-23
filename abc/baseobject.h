#pragma once

class GameSectorView;

class BaseObject
{
public:
	typedef EventHive<BaseObject> EventMgr;

private:
	typedef std::vector<BaseObject*> ObjectList;
	static BaseObject::EventMgr m_objevents;
	static BaseObject::ObjectList m_objlist;

	bool m_is_dead;
	GameSectorPtr m_current_sector;

protected:
	virtual void nextturn(const GameTurn& turn) = 0;

public:
	BaseObject(GameSectorPtr sector);
	virtual ~BaseObject();

	bool is_dead() const;
	void set_dead();

	void move_to_sector(GameSectorPtr sector);
	GameSectorPtr current_sector() const;

	template<class Requestor>
	static EventMgr::EventPtr subscribe(Requestor& req)
	{
		return m_objevents.make_new_event(req);
	}
	static void unsubscribe(EventMgr::EventPtr handle);

	template<class Callback>
	static void for_each_object(Callback& cb)
	{
		std::for_each(m_objlist.begin(), m_objlist.end(), cb);
	}
	static void nextturn_for_all(const GameTurn& turn);
};
