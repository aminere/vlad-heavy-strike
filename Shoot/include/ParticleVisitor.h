/* 

Amine Rehioui
Created: November 3rd 2013

*/

#ifndef _PARTICLE_VISITOR_H_INCLUDED_
#define _PARTICLE_VISITOR_H_INCLUDED_

#include "EntityVisitor.h"

namespace shoot
{
	//! ParticleVisitor class
	class ParticleVisitor : public EntityVisitor
	{
	public:

		DECLARE_OBJECT(ParticleVisitor, EntityVisitor);

		//! constructor
		ParticleVisitor();

		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		void Visit(Entity* pTarget);
	
	protected:
		
		// properties
		bool m_bEmitterActive;		
	};
}

#endif // _PARTICLE_VISITOR_H_INCLUDED_


