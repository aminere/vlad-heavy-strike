/* 

Amine Rehioui
Created: September 9th 2012

*/

#ifndef _RESOURCE_WATCHER_H_INCLUDED_
#define _RESOURCE_WATCHER_H_INCLUDED_

namespace shoot
{
	//! ResourceWatcher class
	class ResourceWatcher : public Singleton<ResourceWatcher>
	{
		DECLARE_SINGLETON(ResourceWatcher);

	public:	

		//! called when a resource changed
		void OnResourceChanged(std::string path);
	};
}

#endif // _RESOURCE_WATCHER_H_INCLUDED_


