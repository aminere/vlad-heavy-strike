/* 

Amine Rehioui
Created: August 22nd 2011

*/

#ifndef _TOOLBAR_MANAGER_H_INCLUDED_
#define _TOOLBAR_MANAGER_H_INCLUDED_

#include "wx/aui/auibar.h"

namespace shoot
{
	//! ToolbarManager
	class ToolbarManager : public Singleton<ToolbarManager>
	{
		DECLARE_SINGLETON(ToolbarManager);

	public:

		//! Initializes the tool bar manager
		void Init();

		//! returns the main tool bar
		wxAuiToolBar* GetMainToolbar() { return m_pMainToolbar; }

	private:

		wxAuiToolBar* m_pMainToolbar;
	};
}

#endif // _TOOLBAR_MANAGER_H_INCLUDED_

