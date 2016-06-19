/* 

Amine Rehioui
Created: September 9th 2012

*/

#include "ShootEditorCommon.h"

#include "EditorUtils.h"
#include "ResourceWatcher.h"

#ifdef __cplusplus_cli
#include <vcclr.h>
#using <System.dll>
using namespace System::IO;
using namespace System;
using namespace System::Runtime::InteropServices;

//! static file change handler
void StaticOnFileChanged(Object^ source, FileSystemEventArgs^ args)
{ 
	char* cstr = (char*)((void*)Marshal::StringToHGlobalAnsi(args->FullPath));
	std::string path;
	if(shoot::EditorUtils::GetRelativePath(cstr, path))
	{
		shoot::ResourceWatcher::Instance()->OnResourceChanged(path);
	}
}
#endif // __cplusplus_cli

namespace shoot
{
	//! constructor
	ResourceWatcher::ResourceWatcher()
	{
#ifdef __cplusplus_cli
		FileSystemWatcher^ watcher = gcnew FileSystemWatcher("./data", "*.*");
		watcher->NotifyFilter = NotifyFilters::LastWrite;
		watcher->IncludeSubdirectories = true;

		// register handler
		watcher->Changed += gcnew FileSystemEventHandler(StaticOnFileChanged);

		// start watching file system
		watcher->EnableRaisingEvents = true;
#endif // __cplusplus_cli
	}

	//! destructor
	ResourceWatcher::~ResourceWatcher()
	{
	}

	//! called when a resource changed
	void ResourceWatcher::OnResourceChanged(std::string path)
	{
		ResourceManager::Instance()->RequestReloadResource(path.c_str());
	}
}

