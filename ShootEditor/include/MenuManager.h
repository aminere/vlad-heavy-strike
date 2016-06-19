/* 

Amine Rehioui
Created: August 22nd 2011

*/

#ifndef _MENU_MANAGER_H_INCLUDED_
#define _MENU_MANAGER_H_INCLUDED_

#include <wx/menu.h>

namespace shoot
{
	//! MenuManager
	class MenuManager : public Singleton<MenuManager>
	{
		DECLARE_SINGLETON(MenuManager);

	public:

		//! Initializes the menu manager
		void Init();

		//! returns the main menu bar
		wxMenuBar* GetMainMenuBar() { return m_pMainMenu; }

		//! returns the type index for a class
		s32 GetTypeIndex(const char* strClassName);

	private:

		//! ClassInfo
		struct ClassInfo
		{
			std::string className;
			u32 typeIndex;
		};

		std::map< std::string, std::vector<ClassInfo> > m_aCategories;

		wxMenuBar* m_pMainMenu;
	};
}

#endif // _MENU_MANAGER_H_INCLUDED_

