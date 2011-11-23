#include "import.h"
#include "all.h"

template<> XmlResourceManager* Ogre::Singleton<XmlResourceManager>::ms_Singleton = 0;

XmlResourceSerializer::XmlResourceSerializer()
{

}

XmlResourceSerializer::~XmlResourceSerializer()
{

}

void XmlResourceSerializer::importXml(Ogre::DataStreamPtr& stream, XmlResource* destination)
{
	destination->m_doc->reset();
	destination->m_doc->load(stream->getAsString().c_str());
}

void XmlResource::loadImpl()
{
	XmlResourceSerializer serializer;
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
	serializer.importXml(stream, this);
}

void XmlResource::unloadImpl()
{
	m_doc->reset();
}

size_t XmlResource::calculateSize() const 
{
	std::stringstream stream;
	m_doc->save(stream);
	return stream.str().length();
}

XmlResource::XmlResource(Ogre::ResourceManager *creator,
	const Ogre::String &name,
	Ogre::ResourceHandle handle,
	const Ogre::String &group,
	bool isManual /*= false*/,
	Ogre::ManualResourceLoader *loader /*= 0*/) :
	Ogre::Resource(creator, name, handle, group, isManual, loader)
{
	m_doc = XmlDocumentPtr(new pugi::xml_document());
	/* If you were storing a pointer to an object, then you would set that pointer to NULL here.
	*/
 
	/* For consistency with StringInterface, but we don't add any parameters here
	That's because the Resource implementation of StringInterface is to
	list all the options that need to be set before loading, of which 
	we have none as such. Full details can be set through scripts.
	*/ 
	createParamDictionary(XML_RESOURCE_TYPE_NAME);
}

XmlResource::~XmlResource()
{
	unload();
}

void XmlResource::setDoc(XmlDocumentPtr doc)
{
	m_doc = doc;
}

XmlDocumentPtr XmlResource::getDoc()
{
	return m_doc;
}

XmlResourcePtr::XmlResourcePtr() :
	Ogre::SharedPtr<XmlResource>()
{

}

XmlResourcePtr::XmlResourcePtr(XmlResource *rep) :
	Ogre::SharedPtr<XmlResource>(rep)
{

}

XmlResourcePtr::XmlResourcePtr(const XmlResourcePtr &r) :
	Ogre::SharedPtr<XmlResource>(r)
{

}

XmlResourcePtr::XmlResourcePtr(const Ogre::ResourcePtr &r) :
	Ogre::SharedPtr<XmlResource>()
{
	if(r.isNull())
	{
		return;
	}
	// lock & copy other mutex pointer
	OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
	OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
	pRep = static_cast<XmlResource*>(r.getPointer());
	pUseCount = r.useCountPointer();
	useFreeMethod = r.freeMethod();
	if (pUseCount)
	{
		++(*pUseCount);
	}
}

XmlResourcePtr& XmlResourcePtr::operator=(const Ogre::ResourcePtr& r)
{
	if(pRep == static_cast<XmlResource*>(r.getPointer()))
	{
		return *this;
	}
	release();
	if(r.isNull())
	{
		return *this; // resource ptr is null, so the call to release above has done all we need to do.
	}
	// lock & copy other mutex pointer
	OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
	OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
	pRep = static_cast<XmlResource*>(r.getPointer());
	pUseCount = r.useCountPointer();
	useFreeMethod = r.freeMethod();
	if (pUseCount)
	{
		++(*pUseCount);
	}
	return *this;
}

Ogre::Resource* XmlResourceManager::createImpl(const Ogre::String &name, 
	Ogre::ResourceHandle handle, 
	const Ogre::String &group, 
	bool isManual,
	Ogre::ManualResourceLoader *loader, 
	const Ogre::NameValuePairList *createParams)
{
	return new XmlResource(this, name, handle, group, isManual, loader);
}

XmlResourceManager& XmlResourceManager::getSingleton()
{
	assert(ms_Singleton);  
	return(*ms_Singleton);
}

XmlResourceManager* XmlResourceManager::getSingletonPtr()
{
     return ms_Singleton;
}

XmlResourceManager::XmlResourceManager()
{
	mResourceType = XML_RESOURCE_TYPE_NAME;

	// low?
	mLoadOrder = 30.0f;

	// this is how we register the ResourceManager with OGRE
	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	Ogre::FileSystemArchiveFactory factory;
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(XML_RESOURCE_DIR,
		factory.getType(),
		XML_RESOURCE_GROUP_NAME,
		true);
}

XmlResourceManager::~XmlResourceManager()
{
	// and this is how we unregister it
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

XmlResourcePtr XmlResourceManager::load(const Ogre::String &name, const Ogre::String &group)
{
	XmlResourcePtr xml = getByName(name);

	if (xml.isNull())
		xml = create(name, group);

	xml->load();
	return xml;
}
