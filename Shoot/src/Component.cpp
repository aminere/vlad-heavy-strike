/*	

Amine Rehioui
Created: January 14th 2012

*/

#include "Shoot.h"

namespace shoot
{
	DEFINE_OBJECT(Component);

	//! constructor
	Component::Component()
		: m_pParent(NULL)
	{
	}

	//! destructor
	Component::~Component()
	{
	}
}

