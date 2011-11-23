#include "import.h"
#include "all.h"

void World::add_sector(GameSectorPtr sector)
{
	auto it = m_sectors.find(sector->name());
	if (it != m_sectors.end())
	{
		throw std::logic_error("2 sectors with same name");
	}
	m_sectors.insert(std::pair<std::string, GameSectorPtr>(sector->name(), sector));
}

World::World(XmlResourceManager& xmlmgr)
{
	load_sectors(xmlmgr);
	load_staticobj(xmlmgr);
}

void World::load_sectors(XmlResourceManager& xmlmgr)
{
	XmlResourcePtr xmlres = xmlmgr.load("world.xml", XML_RESOURCE_GROUP_NAME);
	auto document = xmlres->getDoc();
	auto sectors = document->select_nodes("/document/sector");
	std::for_each(sectors.begin(), sectors.end(), [&](const pugi::xpath_node& node){
		GameSectorPtr ptr(new GameSector(node.node().child_value(), xmlmgr));
		add_sector(ptr);
	});
}

void World::load_staticobj(XmlResourceManager& xmlmgr)
{
	XmlResourcePtr xmlres = xmlmgr.load("staticobj.xml", XML_RESOURCE_GROUP_NAME);
	auto document = xmlres->getDoc();
	auto obj_list = document->select_nodes("/document/object");
	std::for_each(obj_list.begin(), obj_list.end(), [&](const pugi::xpath_node& node){
		GameObjectFactory::instanse().create(node.node(), this);
	});
}

GameSectorPtr World::fetch_sector(const std::string& name)
{
	auto it = m_sectors.find(name);
	if (it == m_sectors.end())
	{
		throw ESearchItem(name.c_str(), "WorldSector");
	}
	return it->second;
}
