#pragma once

class GameSectorView;

class World : public GameSectorStorage
{
	std::hash_map<std::string, GameSectorPtr> m_sectors;

	void add_sector(GameSectorPtr sector);

	void load_sectors(XmlResourceManager& xmlmgr);
	void load_staticobj(XmlResourceManager& xmlmgr);
public:
	World(XmlResourceManager& xmlmgr);
	GameSectorPtr fetch_sector(const std::string& name);
};