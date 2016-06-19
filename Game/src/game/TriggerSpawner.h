/* 

Amine Rehioui
Created: March 3rd 2013

*/

#ifndef _TRIGGER_SPAWNER_H_INCLUDED_
#define _TRIGGER_SPAWNER_H_INCLUDED_

namespace shoot
{
	//! TriggerSpawner class
	class TriggerSpawner : public Entity3D
	{
	public:
		
		DECLARE_OBJECT(TriggerSpawner, Entity3D);

		//! Constructor
		TriggerSpawner();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the update of the entity		
		virtual void Update();

		//! returns the dist to player
		inline f32 GetDistToPlayer() const { return m_fDistToPlayer; }

		//! returns the bounding box
		virtual AABBox3D GetBoundingBox() const;

		//! sets the active status
		inline void SetActive(bool bActive) { m_bActive = bActive; }

		//! attempts to re-activates the trigger
		void Reactivate();

	protected:
		
		bool m_bActive;

		// properties
		f32 m_fDistToPlayer;
		bool m_bCanReactivate;
		Reference<Actor> m_Actor;
		Reference<ActorSettings> m_ActorSettings;
	};
}

#endif // _TRIGGER_SPAWNER_H_INCLUDED_

