#include "import.h"
#include "all.h"

Ogre::String Utl::create_hex_name(void* ptr)
{
	std::stringstream objname;
	objname << std::hex << ptr;
	return objname.str();
}
