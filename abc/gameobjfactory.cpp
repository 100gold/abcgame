#include "import.h"
#include "all.h"

GameObjectFactory& GameObjectFactory::instanse()
{
	static GameObjectFactory factory;
	return factory;
}

GameObjectFactory::GameObjectFactory()
{

}

void GameObjectFactory::unreg_object(const std::string& name)
{
	m_object_creators.erase(name);
}

BaseObject* GameObjectFactory::create(const pugi::xml_node& xml, GameSectorStorage* sect_storage)
{
	auto it = m_object_creators.find(xml.attribute("type").value());
	if (it == m_object_creators.end())
	{
		throw ESearchItem(xml.attribute("type").value(), "GameObjectFactoryCreators");
	}
	auto sector_name = xml.child("sector");
	if (NULL == sector_name)
	{
		throw ESearchItem("no <sector> tag", "some object");
	}
	auto target_sector = sect_storage->fetch_sector(sector_name.child_value());
	BaseObject* result = it->second->create(xml, target_sector);
	target_sector->arrive(result);
	return result;
}

void GameObjectFactory::register_object(const std::string& name, ConreteGameObjectCreatorPtr creator)
{
	m_object_creators.insert(std::make_pair(name, creator));
}

