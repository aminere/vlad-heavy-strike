/* 

Amine Rehioui
Created: April 1st 2011

*/

#ifndef _CONSOLE_H_INCLUDED_
#define _CONSOLE_H_INCLUDED_

namespace shoot
{
	//! Console
	class Console : public wxFrame
	{
		typedef wxFrame super;

	public:
		
		//! Constructor
		Console();

		//! Destructor
		virtual ~Console();	

		//! prints a string into the console		
		void Print(const char* strMessage);

		//! returns the static instance
		inline static Console* Instance() { return ms_pInstance; }

	private:

		wxTextCtrl* m_pTextControl;

		// event handlers
		void OnClear(wxCommandEvent& event);

		static Console* ms_pInstance;

		DECLARE_EVENT_TABLE();
	};
}

#endif // _CONSOLE_H_INCLUDED_


