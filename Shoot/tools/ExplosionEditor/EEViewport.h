/* 

Amine Rehioui
Created: December 25th 2012

*/

#ifndef _EXPLOSION_EDITOR_VIEWPORT_H_INCLUDED_
#define _EXPLOSION_EDITOR_VIEWPORT_H_INCLUDED_

#include "wx/glcanvas.h"

#include "ExplosionVisitor.h"

using namespace shoot;

//! The rendering area of the editor
class EEViewPort : public wxGLCanvas
{
	typedef wxGLCanvas super;

public:
	
	//! Constructor
	EEViewPort(wxWindow *parent);

	//! Destructor
	virtual ~EEViewPort();

private:

	//! returns the index of a triangle in a chunk, -1 if not found
	s32 GetTriangleIndex(ExplosionVisitor::Chunk& chunk, u32 triangle);

	//! returns the index of a chunk containing a triangle, -1 if not found
	s32 GetChunkIndex(Array<ExplosionVisitor::Chunk>& aChunks, u32 triangle);

	bool m_bZoomingStarted;
	bool m_bLookingStarted;
	Vector2 m_LastMousePosition;
	s32 m_SelectedTriangleIndex;
	s32 m_SelectedSubMeshIndex;

	// event handlers
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMiddleDown(wxMouseEvent& event);
	void OnMouseMiddleUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseWheelRolled(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnResized(wxSizeEvent& event);
	void OnFocusGained(wxFocusEvent& event);
	void OnFocusLost(wxFocusEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	DECLARE_EVENT_TABLE();
};

#endif // _EXPLOSION_EDITOR_VIEWPORT_H_INCLUDED_

