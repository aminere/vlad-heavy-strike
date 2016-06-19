/* 

Amine Rehioui
Created: July 14th 2013

*/

#ifndef _GRAPHIC_OBJECT_H_INCLUDED_
#define _GRAPHIC_OBJECT_H_INCLUDED_

namespace shoot
{
	//! GraphicObject
	class GraphicObject
	{
	public:

		//! constructor
		GraphicObject();

		//! destructor
		virtual ~GraphicObject();

		//! Load into video memory
		virtual void GraphicLoad() { }

		//! Unload from video memory
		virtual void GraphicUnload() { }

		//! returns loaded status
		virtual bool IsLoaded() { return true; }
	};
}

#endif // _GRAPHIC_OBJECT_H_INCLUDED_

