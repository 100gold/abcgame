#pragma once

class BaseObject;

class ConreteGameObjectCreator
{
public:
	virtual BaseObject* create(const pugi::xml_node& xml, GameSectorPtr target_sector) const = 0;
};
typedef boost::shared_ptr<ConreteGameObjectCreator> ConreteGameObjectCreatorPtr;

template<class Target>
class DefaultGameObjectCreator : public ConreteGameObjectCreator
{
public:
	BaseObject* create(const pugi::xml_node& xml, GameSectorPtr target_sector) const
	{
		return new Target(target_sector, xml);
	}
};

class GameObjectFactory
{
	std::hash_map<std::string, ConreteGameObjectCreatorPtr> m_object_creators;

	GameObjectFactory();
	GameObjectFactory(const GameObjectFactory&);
	GameObjectFactory& operator=(GameObjectFactory&);
public:
	static GameObjectFactory& instanse();

	BaseObject* create(const pugi::xml_node& xml, GameSectorStorage* sect_storage);

	void register_object(const std::string& name, ConreteGameObjectCreatorPtr creator);
	void unreg_object(const std::string& name);
};

template<class CustomGameObject, class CustomGameObjectCreator = DefaultGameObjectCreator<CustomGameObject>>
class GameObjectFactoryAutoRegistrator
{
	struct RunOnce
	{
		RunOnce()
		{
			static ConreteGameObjectCreatorPtr ptr(new CustomGameObjectCreator());
			GameObjectFactory::instanse().register_object(CustomGameObject::object_class_name(), ptr);
		}
	};
public:
	GameObjectFactoryAutoRegistrator()
	{
		static RunOnce once;
	}
};
#define DECLARE_GAMEOBJECT_NAME(_NAME_) \
	static const char* object_class_name() \
	{ \
		return _NAME_; \
	}
#define AUTOREGISTER_GAMEOBJECT(_CLASS_) \
	namespace __AutoregisterGameobject\
	{ \
		static GameObjectFactoryAutoRegistrator<_CLASS_> g_runonce##_CLASS_; \
	}