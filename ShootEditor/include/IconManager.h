/* 

Amine Rehioui
Created: August 20th 2011

*/

#ifndef _ICON_MANAGER_H_INCLUDED_
#define _ICON_MANAGER_H_INCLUDED_

#include <wx/imaglist.h>
#include <wx/bitmap.h>

namespace shoot
{
	// forwards
	class Object;

	//! IconManager
	class IconManager : public Singleton<IconManager>
	{
		DECLARE_SINGLETON(IconManager);

	public:

		//! icon types
		enum IconType
		{
			Icon_Translate,
			Icon_Rotate,
			Icon_Scale,
			Icon_Pin,
			Icon_PostRendering,
			Icon_Audio,
			Icon_3D,
			Icon_Add,
			Icon_Remove,
			Icon_Play,
			Icon_Pause,
			Icon_Stop,
			Icon_Undo,
			Icon_Redo,
			Icon_ArrowUp,
			Icon_ArrowDown,
		};

		//! gets the wx image list
		wxImageList* GetImageList() { return m_pImageList; }

		//! returns the icon index within the image list for a particular object		
		u32 GetIconIndex(Object* pObject);

		//! returns the icon corresponding to an entity
		wxBitmap GetBitmap(const std::string& strClassName);

		//! returns an icon by type
		wxBitmap GetBitmap(IconType type);

	private:

		wxImageList* m_pImageList;

		std::map<std::string, wxBitmap> m_IconMap;
	};
}

#endif // _ICON_MANAGER_H_INCLUDED_


