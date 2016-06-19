/* 

Amine Rehioui
Created: October 27th 2013

*/

#ifndef _MATERIAL_VISITOR_H_INCLUDED_
#define _MATERIAL_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

namespace shoot
{
	//! MaterialVisitor class
	class MaterialVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(MaterialVisitor, AnimationVisitor);

		//! constructor
		MaterialVisitor();

		//! destructor
		virtual ~MaterialVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);
	
		//! updates the visitor
		virtual void Update();

	protected:
		
		f32 m_fInterpolator;

		//! updates the color
		void UpdateColor(const Color& color);

		// properties
		Color m_SrcColor;
		Color m_DestColor;
		f32 m_fDuration;
	};
}

#endif // _MATERIAL_VISITOR_H_INCLUDED_


