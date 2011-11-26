#include "import.h"
#include "all.h"

namespace Math
{

Ogre::Real moreeq(const Ogre::Real& a, const Ogre::Real& b)
{
	return a-b > -std::numeric_limits<Ogre::Real>::epsilon();
}

}
