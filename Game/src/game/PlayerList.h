/* 

Amine Rehioui
Created: September 21st 2013

*/

#ifndef _PLAYER_LIST_H_INCLUDED_
#define _PLAYER_LIST_H_INCLUDED_

namespace shoot
{
	//! PlayerList
	class PlayerList : public Entity2D				 
	{	
		DECLARE_OBJECT(PlayerList, Entity2D);

	public:

		//! Constructor
		PlayerList();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity
		virtual void Update();

		//! fills the list
		void Fill(bool bInitScrollPos = false);

	private:

		Vector2 m_vPreviousTouchPos;
		f32 m_fTouchYAccumulator;
		s32 m_ScrollPos;
		Vector2 m_vOriginalPosition;
		bool m_bScrollingStarted;
		Handle<Entity2D> m_Highlighter;

		// properties
		f32 m_fScrollTreshold;
	};
}

#endif // _PLAYER_LIST_H_INCLUDED_

