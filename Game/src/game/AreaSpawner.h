/* 

Amine Rehioui
Created: November 05th 2011

*/

#ifndef _AREA_SPAWNER_H_INCLUDED_
#define _AREA_SPAWNER_H_INCLUDED_

namespace shoot
{
	//! AreaSpawner class
	class AreaSpawner : public Entity3D
	{
	public:
		
		DECLARE_OBJECT(AreaSpawner, Entity3D);

		//! Constructor
		AreaSpawner();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the update of the entity		
		virtual void Update();

		//! returns the activation radius
		f32 GetRadius() const { return m_fRadius; }

		//! returns the bounding box
		virtual AABBox3D GetBoundingBox() const;

	protected:

		//! spawns the actors
		void Spawn(ActorSettings* pSettings);
		
		f32 m_fTimer;
		bool m_bActive;

		// properties
		Reference<Actor> m_Actor;
		Reference<ActorSettings> m_ActorSettings;
		f32 m_fRadius;
		f32 m_fFrequency;
	};
}

#endif // _AREA_SPAWNER_H_INCLUDED_


