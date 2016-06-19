/* 

Amine Rehioui
Created: February 5th 2012

*/

#ifndef _PULSE_MANAGER_H_INCLUDED_
#define _PULSE_MANAGER_H_INCLUDED_

#include "BulletManager.h"

namespace shoot
{
	// forwards
	class VertexBuffer;

	//! PulseManager class
	class PulseManager : public BulletManager
	{
	public:
		
		DECLARE_OBJECT(PulseManager, BulletManager);

		//! Constructor
		PulseManager();

		//! destructor
		virtual ~PulseManager();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! sets the bullet size
		void SetBulletSize(f32 fSize);

	private:

		//! Prepares a bullet for rendering
		void SetupRendering(Bullet* pBullet, u32 index, VertexBuffer* pVB);

		//! properties
		f32 m_fBulletSize;
	};
}

#endif // _PULSE_MANAGER_H_INCLUDED_


