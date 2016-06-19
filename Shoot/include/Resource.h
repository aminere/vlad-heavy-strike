/* 

Amine Rehioui
Created: June 13th 2010

*/

#ifndef _RESOURCE_H_INCLUDED_
#define _RESOURCE_H_INCLUDED_

#include "Utils.h"

namespace shoot
{
	//! Resource interface
	class Resource : public Object
	{
		DECLARE_OBJECT(Resource, Object);

	public:

		//! destructor
		virtual ~Resource();

		//! Load from disk
		virtual void ResourceInit() { }

		//! Unload disk data
		virtual void ResourceUninit() { }

		//! returns the memory size in bytes
		virtual u32 GetMemorySize() { return 0; }

		//! returns true if this resource is using the specified data
		virtual bool IsUsingData(const char* strPath) const { return Utils::Equals(GetTemplatePath().c_str(), strPath); }
	};
}

#endif // _RESOURCE_H_INCLUDED_

