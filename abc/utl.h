#pragma once

namespace Utl
{

Ogre::String create_hex_name(void* ptr);


template<class Target, class Source>
Target* dynamic_cast_shared_ptr(boost::shared_ptr<Source> source)
{
	Target* result = dynamic_cast<Target*>(source.get());
	if (NULL == result)
	{
		throw ERootException();
	}
	return result;
}

}

