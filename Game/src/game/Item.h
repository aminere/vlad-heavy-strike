/* 

Amine Rehioui
Created: April 23nd 2013

*/

#ifndef _ITEM_H_INCLUDED_
#define _ITEM_H_INCLUDED_

namespace shoot
{
	//! item class
	class Item : public Entity3D
	{
		DECLARE_OBJECT(Item, Entity3D);

	public:

		//! constructor
		Item();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the update of the entity
		void Update(); 

		//! items type
		enum E_Type
		{
			T_PulseUp,
			T_LaserUp,
			T_PelletUp,
			T_Gold
		};

		//! returns the item type
		inline E_Type GetType() const { return m_eType; }

	private:

		f32 m_fTimer;

		// properties
		E_Type m_eType;
	};
}

#endif // _ITEM_H_INCLUDED_

