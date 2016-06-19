/* 

Amine Rehioui
Created: August 5th 2013

*/

#ifndef _SQUAD_H_INCLUDED_
#define _SQUAD_H_INCLUDED_

#include "BasicFighter.h"

namespace shoot
{
	//! SquadSettings
	class SquadSettings : public BasicFighterSettings
	{
		DECLARE_OBJECT(SquadSettings, BasicFighterSettings);
	};


	//! Squad class
	class Squad : public Actor
	{
	public:
		
		DECLARE_OBJECT(Squad, Actor);

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

	private:

		std::vector< Handle<Entity> > m_Fighters;
	};
}

#endif // _SQUAD_H_INCLUDED_


