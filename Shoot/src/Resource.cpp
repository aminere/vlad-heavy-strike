/* 

Amine Rehioui
Created: January 4th 2012

*/

#include "Shoot.h"

namespace shoot
{
	DEFINE_OBJECT(Resource);

	//! destructor
	Resource::~Resource()
	{
		ResourceManager::Instance()->OnResourceDestroyed(this);
	}
}

