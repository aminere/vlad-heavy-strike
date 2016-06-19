/* 

Amine Rehioui
Created: April 9th 2011

*/

#ifndef _COLOR_VISITOR_H_INCLUDED_
#define _COLOR_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "VertexBuffer.h"

namespace shoot
{
	//! ColorVisitor class
	class ColorVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(ColorVisitor, AnimationVisitor);

		//! constructor
		ColorVisitor();

		//! destructor
		virtual ~ColorVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);
	
		//! Leaves the visited entity
		virtual void Leave();

		//! updates the visitor
		virtual void Update();

	protected:
		
		std::vector< Reference<VertexBuffer> > m_VertexBuffers;
		std::vector< Reference<VertexBuffer> > m_OriginalVertexBuffers;

		f32 m_fInterpolator;	
		Color m_SrcColor;
		Color m_DestColor;

		//! updates the color
		void UpdateColor(const Color& color);

		// properties
		Color m_Color;
		f32 m_fAlpha;
		f32 m_fDuration;
	};
}

#endif // _COLOR_VISITOR_H_INCLUDED_


