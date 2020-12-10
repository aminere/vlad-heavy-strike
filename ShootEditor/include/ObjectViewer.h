/* 

Amine Rehioui
Created: March 31st 2011

*/

#ifndef _OBJECT_VIEWER_H_INCLUDED_
#define _OBJECT_VIEWER_H_INCLUDED_

#include <wx/listctrl.h>

namespace shoot
{
	//! Object Viewer
	class ObjectViewer : public wxPanel
	{
		typedef wxPanel super;

	public:
		
		//! Constructor
		ObjectViewer(wxWindow* pParent);

		//! Destructor
		virtual ~ObjectViewer();	

		//! adds an object
		void AddObject(Object* pObject);

	private:

		wxListView* m_pObjectList;
		ObjectManager::ObjectMap m_PreviousMap;

		// column sorting
		static const u32 NumColumns = 5;
		s32 m_ColumnToSort;
		bool m_ColumSortDescending[NumColumns];
		static int wxCALLBACK ObjectViewerSort(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);

		//! event handlers
		void OnColumnClick(wxListEvent& event);
		void OnItemSelected(wxListEvent& event);
		void OnIdle(wxIdleEvent& event);

		DECLARE_EVENT_TABLE();
	};
}

#endif // _OBJECT_VIEWER_H_INCLUDED_


