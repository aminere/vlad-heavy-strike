/* 

Amine Rehioui
Created: October 15th 2011

*/

#ifndef _SHOOT_TEST_EDITOR_H_INCLUDED_
#define _SHOOT_TEST_EDITOR_H_INCLUDED_

#include "ShootEditor.h"

namespace shoot
{
	//! custom ShootTest editor
	class ShootTestEditor : public ShootEditor
	{
		typedef ShootEditor super;

	public:

		//! constructor
		ShootTestEditor();

		//! destructor
		virtual ~ShootTestEditor();

		//! returns a menu category for a class type
		std::string GetMenuCategory(const char* classType);

		//! Selects an entity
		Entity* SelectEntity(Entity* pEntity, const Vector3& vRayStart, const Vector3& vRayDir);
	};
}

#endif // _SHOOT_TEST_EDITOR_H_INCLUDED_

