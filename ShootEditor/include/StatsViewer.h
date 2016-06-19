/* 

Amine Rehioui
Created: March 31st 2011

*/

#ifndef _STATS_VIEWER_H_INCLUDED_
#define _STATS_VIEWER_H_INCLUDED_

namespace shoot
{
	//! Stats Viewer
	class StatsViewer : public wxFrame
	{
		typedef wxFrame super;

	public:
		
		//! Constructor
		StatsViewer();

		//! Destructor
		virtual ~StatsViewer();	

		//! returns the static instance
		inline static StatsViewer* Instance() { return ms_pInstance; }

	private:

		static StatsViewer* ms_pInstance;
	};
}

#endif // _STATS_VIEWER_H_INCLUDED_


