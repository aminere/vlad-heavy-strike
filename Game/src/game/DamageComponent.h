/* 

Amine Rehioui
Created: October 20th 2013

*/

#ifndef _DAMAGE_COMPONENT_H_INCLUDED_
#define _DAMAGE_COMPONENT_H_INCLUDED_

#include "RigidBodyComponent.h"

namespace shoot
{
	//! DamageComponent
	class DamageComponent : public RigidBodyComponent
	{
		DECLARE_OBJECT(DamageComponent, RigidBodyComponent);

	public:

		//! constructor
		DamageComponent();

		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);

		//! returns the damage per second
		inline f32 GetDamagePerSecond() const { return m_fDamagePerSecond; }

		//! get explode on contact status
		inline bool GetExplodeOnContact() const { return m_bExplodeOnContact; }

	private:

		// properties
		f32 m_fDamagePerSecond;
		bool m_bExplodeOnContact;
	};
}

#endif // _DAMAGE_COMPONENT_H_INCLUDED_

