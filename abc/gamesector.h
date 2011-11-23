#pragma once

class GameSectorView;
class BaseObject;


class GameSector
{
public:
	typedef std::vector<BaseObject*> BaseObjectList;

private:
	std::string m_name;
	std::string m_display_name;
	BaseObjectList m_base_objects;
	GameSectorView* m_game_view;
public:
	GameSector(const std::string& config, XmlResourceManager& xmlmgr);

	const std::string& name();
	const std::string& display_name();

	void arrive(BaseObject* object);
	void leave(BaseObject* object);

	BaseObjectList::iterator begin();
	BaseObjectList::iterator end();

	void hide(GameSectorView* view);
	void show(GameSectorView* view);
};

typedef boost::shared_ptr<GameSector> GameSectorPtr;

class GameSectorStorage
{
public:
	virtual GameSectorPtr fetch_sector(const std::string& name) = 0;
};
