#pragma once

struct TestGameObjectFixture : public GameSectorStorage
{
	typedef std::map<std::string,std::string> SectorMap;

	class XmlResourceManagerFixture : public XmlResourceManager
	{
		SectorMap& m_internal_map;
	public:

		XmlResourceManagerFixture(SectorMap& map) :
			m_internal_map(map),
			XmlResourceManager()
		{
		}
		virtual XmlResourcePtr load(const Ogre::String &name, const Ogre::String &group)
		{
			XmlResourcePtr result(new XmlResource(this, name, 0, group));
			XmlDocumentPtr doc(new pugi::xml_document());
			auto it = m_internal_map.find(name);
			doc->load(it->second.c_str());
			result->setDoc(doc);
			return result;
		}
	};

	std::map<std::string, GameSectorPtr> m_sectors;
	XmlResourceManagerFixture *m_xmlmgr;
	OgreBase m_ogreroot;

	TestGameObjectFixture(SectorMap& map)
	{
		m_xmlmgr = new XmlResourceManagerFixture(map);
		std::for_each(map.begin(), map.end(), [&](SectorMap::value_type& pair){
			m_sectors.insert(std::make_pair(
				pair.first,
				GameSectorPtr(new GameSector(pair.first, *m_xmlmgr))
				));
		});
	}
	~TestGameObjectFixture()
	{
		delete m_xmlmgr;
	}

	GameSectorPtr fetch_sector(const std::string& name)
	{
		auto it = m_sectors.find(name);
		return it->second;
	}
};