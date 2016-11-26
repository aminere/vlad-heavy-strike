/* 

Amine Rehioui
Created: August 9th 2010

*/

#ifndef _VIEWPORT_CONTAINER_H_INCLUDED_
#define _VIEWPORT_CONTAINER_H_INCLUDED_

#include "ViewPort.h"

namespace shoot
{
	//! Container for the viewport
	class ViewPortContainer : public wxPanel							   
	{
		typedef wxPanel super;

	public:
		
		//! Constructor
		ViewPortContainer(wxWindow *parent);

		//! returns the view port
		ViewPort* GetViewPort() { return m_pViewPort; }

		// event handlers
		void OnResized(wxSizeEvent& event);

	private:

		ViewPort* m_pViewPort;

		DECLARE_EVENT_TABLE();
	};
}

#endif // _VIEWPORT_CONTAINER_H_INCLUDED_

