#pragma once

#define XML_RESOURCE_GROUP_NAME "Xml"
#define XML_RESOURCE_TYPE_NAME "XmlResource"
#define XML_RESOURCE_DIR "xml"

typedef boost::shared_ptr<pugi::xml_document> XmlDocumentPtr;

class XmlResource;

class XmlResourceSerializer : public Ogre::Serializer
{
public:
	XmlResourceSerializer();
	virtual ~XmlResourceSerializer();
	void importXml(Ogre::DataStreamPtr& stream, XmlResource* destination);
};

class XmlResource : public Ogre::Resource
{
	friend XmlResourceSerializer;
	XmlDocumentPtr m_doc;

protected:
	// must implement these from the Ogre::Resource interface
	void loadImpl();
	void unloadImpl();
	size_t calculateSize() const;

public:

	XmlResource(Ogre::ResourceManager *creator, 
		const Ogre::String &name, 
		Ogre::ResourceHandle handle,
		const Ogre::String &group,
		bool isManual = false, 
		Ogre::ManualResourceLoader *loader = 0);
	virtual ~XmlResource();
	void setDoc(XmlDocumentPtr doc);
	XmlDocumentPtr getDoc();
};

class XmlResourcePtr : public Ogre::SharedPtr<XmlResource> 
{
public:
	XmlResourcePtr();
	explicit XmlResourcePtr(XmlResource *rep);
	XmlResourcePtr(const XmlResourcePtr &r);
	XmlResourcePtr(const Ogre::ResourcePtr &r);
	/// Operator used to convert a ResourcePtr to a XmlResourcePtr
	XmlResourcePtr& operator=(const Ogre::ResourcePtr& r);
};

class XmlResourceManager : public Ogre::ResourceManager, public Ogre::Singleton<XmlResourceManager>
{
protected:
	// must implement this from ResourceManager's interface
	Ogre::Resource* createImpl(const Ogre::String &name,
		Ogre::ResourceHandle handle, 
		const Ogre::String &group,
		bool isManual,
		Ogre::ManualResourceLoader *loader, 
		const Ogre::NameValuePairList *createParams);

	XmlResourceManager(XmlResourceManager&);
	XmlResourceManager& operator=(XmlResourceManager&);

public:
	XmlResourceManager();
	virtual ~XmlResourceManager();

	virtual XmlResourcePtr load(const Ogre::String &name, const Ogre::String &group);

	static XmlResourceManager &getSingleton();
	static XmlResourceManager *getSingletonPtr();
};