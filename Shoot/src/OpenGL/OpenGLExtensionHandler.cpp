/* 

Amine Rehioui
Created: August 15th 2010

Uses code from the Irrlicht engine extension handler

*/

#include "Shoot.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! Constructor
	OpenGLExtensionHandler::OpenGLExtensionHandler(const char* const strExtensionStrings[E_Count])	
	{
		const char* strExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
		u32 length = (u32)strlen(strExtensions);

		std::string extension;

		for(u32 i=0; i<length; ++i)
		{			
			if(strExtensions[i] == ' ')
			{

#ifndef _RETAIL_
				Log << "GL EXTENSION: " << extension.c_str() << "\n";
#endif // _RETAIL_

				for(u32 j=0; j<E_Count; ++j)
				{
					if(extension == strExtensionStrings[j])
					{
						m_bHasExtension[j] = true;
						break;
					}
				}

				extension = "";
			}
			else
			{
				extension = extension + strExtensions[i];
			}
		}
		
		for(u32 i=0; i<E_Count; ++i)
		{
			if(std::string(strExtensionStrings[i]) == "Native")
			{
				m_bHasExtension[i] = true;
			}
		}
	}	
}

