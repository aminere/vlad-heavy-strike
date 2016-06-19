/* 

Amine Rehioui
Created: May 1st 2011

*/

#include "ShootEditorCommon.h"

#include "EditorUtils.h"

#include <wx/dir.h>
#include <wx/arrstr.h>

namespace shoot
{
	//! Browse for a file utility
	/** return: true if the user completed the browse operation */
	bool EditorUtils::OpenFileDialog(wxWindow* pParent, std::string& strOut, const char* strFilter /*= NULL*/, const char* strTitle /*= "Open File"*/, long style /*= wxFD_OPEN*/)
	{
		std::string filter = "All files (*.*)|*.*";
		if(strFilter)
		{
			filter =  strFilter + std::string("|") + filter;
		}

		wxFileDialog dialog(pParent, strTitle, ".", "", filter.c_str(), style);
		dialog.CentreOnParent();

		if(dialog.ShowModal() == wxID_OK)
		{
			std::string path = dialog.GetPath().c_str();
			bool bConverted = GetRelativePath(path, strOut);
			if(!bConverted)
			{
				wxMessageBox("Resource must be in a data folder", "Invalid resource path", wxOK | wxICON_ERROR, pParent);
			}
			return bConverted;
		}
		else
		{
			return false;
		}
	}	

	//! formats an object path
	void EditorUtils::FormatObjectPath(Object* pObject, std::string& strPath)
	{
		// force forward slashes and remove the .\ if any
		wxString wxStr(strPath);
		wxStr.Replace(".\\", "");
		wxStr.Replace("\\", "/");
		strPath = wxStr.c_str();
	}

	//! returns a relative path into the data folder
	bool EditorUtils::GetRelativePath(const std::string& strPath, std::string& strOut)
	{
		// convert to relative path
		std::string dataFolder ("data");
		size_t dataKeywordIndex = strPath.find(dataFolder);			
		if(dataKeywordIndex != std::string::npos)
		{
			strOut = strPath.substr(dataKeywordIndex+5);

			// force forward slashes
			wxString wxStr(strOut);
			wxStr.Replace("\\", "/");
			strOut = wxStr.c_str();
			return true;
		}
		else
		{			
			return false;
		}			
	}

	//! converts from a wx key code to a shoot key code
	InputManager::E_KeyType EditorUtils::GetShootKeyType(s32 wxKeyCode)
	{		   
		switch(wxKeyCode)
		{
			case WXK_LEFT:
			case WXK_NUMPAD4:
				return InputManager::KT_Left;

			case WXK_RIGHT:
			case WXK_NUMPAD6:
				return InputManager::KT_Right;

			case WXK_UP:
			case WXK_NUMPAD8:
				return InputManager::KT_Up;

			case WXK_DOWN:
			case WXK_NUMPAD5:
				return InputManager::KT_Down;

			case WXK_ESCAPE: return InputManager::KT_Exit;
			case WXK_BACK: return InputManager::KT_Back;

			case 'a':
			case 'A': 
				return InputManager::KT_A;

			case 'b':
			case 'B': 
				return InputManager::KT_B;

			case 'x':
			case 'X': 
				return InputManager::KT_X;

			case 'y':
			case 'Y': 
				return InputManager::KT_Y;

			case 'l':
			case 'L': 
				return InputManager::KT_L;

			case 'r':
			case 'R': 
				return InputManager::KT_R;	

			default: return InputManager::KT_None;
		}		
	}
}
