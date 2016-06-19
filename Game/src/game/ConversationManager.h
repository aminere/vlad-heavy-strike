/* 

Amine Rehioui
Created: June 10th 2013

*/

#ifndef _CONVERSATION_MANAGER_H_INCLUDED_
#define _CONVERSATION_MANAGER_H_INCLUDED_

namespace shoot
{
	// forwards
	class Entity2D;
	class Text;

	//! ConversationManager
	class ConversationManager : public Entity
	{
		DECLARE_OBJECT(ConversationManager, Entity);		

	public:

		//! Type
		enum E_Type
		{
			T_Enemy,
			T_Player,
			T_Count
		};

		//! Phrase
		struct Phrase : public ISerializableStruct
		{
			//! constructor
			Phrase()
				: m_eType(T_Enemy)
				, m_fDuration(2.0f)
			{
			}

			//! constructor
			Phrase(E_Type eType, const std::string& strText, f32 fDuration)
				: m_eType(eType)
				, m_strText(strText)
				, m_fDuration(fDuration)
			{
			}

			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream);			

			E_Type m_eType;
			std::string m_strText;
			f32 m_fDuration;
		};

		//! constructor
		ConversationManager();

		//! destructor
		virtual ~ConversationManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity
		void Update();

		//! Add a phrase
		void AddPhrase(E_Type eType, const std::string& strText, f32 fDuration);

		//! Adds a phrase
		inline void AddPhrase(const Phrase& phrase) { m_lPhrases.push_back(phrase); }

		//! Start the conversation
		void Start();

		//! returns the status
		inline bool IsActive() const { return m_bActive; }
			
		//! returns the instance
		inline static ConversationManager* Instance()
		{
			return ms_pInstance;
		}

	private:

		std::list<Phrase> m_lPhrases;
		f32 m_fCurrentPhraseDuration;
		Handle<Entity2D> m_TextEntity[T_Count];
		Handle<Text> m_TextBox[T_Count];
		bool m_bActive;

		static ConversationManager* ms_pInstance;
	};
}

#endif // _CONVERSATION_MANAGER_H_INCLUDED_

