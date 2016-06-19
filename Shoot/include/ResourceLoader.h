/* 

Amine Rehioui
Created: November 19th 2011

*/

#ifndef _RESOURCE_LOADER_H_INCLUDED_
#define _RESOURCE_LOADER_H_INCLUDED_

namespace shoot
{
	//! resource loader interface
	class ResourceLoader
	{
	public:

		//! loads a resource from a file
		virtual Resource* Load(const char* strPath) = 0;	
	};
}

#endif // _RESOURCE_LOADER_H_INCLUDED_

