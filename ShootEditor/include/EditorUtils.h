/* 

Amine Rehioui
Created: May 1st 2011

*/

#ifndef _EDITOR_UTILS_H_INCLUDED_
#define _EDITOR_UTILS_H_INCLUDED_

#include "InputManager.h"

namespace shoot
{
	//! Editor utilities class
	class EditorUtils
	{
	public:

		//! Browse for a file utility
		/** return: true if the user completed the browse operation */
		static bool OpenFileDialog(wxWindow* pParent, std::string& strOut, const char* strFilter = NULL, const char* strTitle = "Open File", long style = wxFD_OPEN);

		//! formats an object path
		static void FormatObjectPath(Object* pObject, std::string& strPath);

		//! converts from a wx key code to a shoot key code
		static InputManager::E_KeyType GetShootKeyType(s32 wxKeyCode);

		//! returns a relative path into the data folder
		static bool GetRelativePath(const std::string& strPath, std::string& strOut);
	};
}

#endif // _EDITOR_UTILS_H_INCLUDED_

