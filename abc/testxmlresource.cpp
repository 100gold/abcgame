#include "import.h"
#include "all.h"
#include <boost/test/unit_test.hpp>

namespace
{

BOOST_AUTO_TEST_SUITE(xmlresource_suite)

BOOST_AUTO_TEST_CASE(xmlresource_test0)
{
	OgreBase ogre_root;
	XmlResourceManager xml_resmgr;
	XmlResourcePtr xmlres1 = xml_resmgr.load("hello1.xml", XML_RESOURCE_GROUP_NAME);
	XmlResourcePtr xmlres2 = xml_resmgr.load("hello2.xml", XML_RESOURCE_GROUP_NAME);
	
	XmlDocumentPtr doc1 = xmlres1->getDoc();
	XmlDocumentPtr doc2 = xmlres2->getDoc();

	std::stringstream stream1;
	std::stringstream stream2;
	doc1->save(stream1);	
	doc2->save(stream2);

	BOOST_CHECK(stream1.str() == stream2.str());

	// Reload the file
	xmlres1->reload();

	// unload/remove the file
	xml_resmgr.unload("hello1.xml");
	xml_resmgr.remove("hello1.xml");
	xml_resmgr.unload("hello2.xml");
	xml_resmgr.remove("hello2.xml");
}

BOOST_AUTO_TEST_SUITE_END()

}
