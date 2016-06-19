/* 

Amine Rehioui
Created: April 16th 2011

*/

#include "Shoot.h"

namespace shoot
{	
	//! constructor
	IReference::IReference()
		: m_pObject(NULL)
	{
	}

	//! Sets the referenced object
	void IReference::SetObject(Object* pObject)
	{
		if(m_pObject != pObject)
		{
			if(m_pObject)
			{
				ObjectManager::Instance()->UnregisterReference(m_pObject);
			}
			if(pObject)
			{
				ObjectManager::Instance()->RegisterReference(pObject);
			}
			m_pObject = pObject;
		}
	}
}

