/* 

Amine Rehioui
Created: November 05th 2011

*/

#ifndef _BASIC_ENEMY_H_INCLUDED_
#define _BASIC_ENEMY_H_INCLUDED_

namespace shoot
{
	//! BasicEnemy settings
	class BasicEnemySettings : public ActorSettings
	{
		DECLARE_OBJECT(BasicEnemySettings, ActorSettings);

	public:

		//! constructor
		BasicEnemySettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fSpeed;
		f32 m_fHomingFactor;
	};

	//! Base BasicEnemy class
	class BasicEnemy : public Actor
	{
	public:
		
		DECLARE_OBJECT(BasicEnemy, Actor);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	};
}

#endif // _BASIC_ENEMY_H_INCLUDED_


