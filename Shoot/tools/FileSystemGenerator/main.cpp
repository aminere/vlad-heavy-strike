/* 

Amine Rehioui
Created: April 28th 2012

*/

#include "Shoot.h"
#include "FileSystemGenerator.h"
#include "MemoryManager.h"
#include "FBXMeshLoader.h"

#include "File.h"

int main(int argc, char** argv)
{
	shoot::File::SetUseDataFolder(true);
	shoot::MemoryManager::Init();
	shoot::ObjectManager::CreateInstance();
	shoot::ResourceManager::CreateInstance();

#ifdef DX11
	shoot::GraphicsDriver::CreateInstance(shoot::GraphicsDriver::DT_DirectX11);
#else
	shoot::GraphicsDriver::CreateInstance(shoot::GraphicsDriver::DT_OpenGL);
#endif

	shoot::FileSystemGenerator().Generate();

	shoot::GraphicsDriver::DestroyInstance();
	shoot::ResourceManager::DestroyInstance();
	shoot::ObjectManager::DestroyInstance();
	shoot::FBXMeshLoader::Uninit();
	shoot::MemoryManager::Uninit();

	shoot::Log << "Success!\n";
	return 0;
}

