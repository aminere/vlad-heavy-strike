/* 

Amine Rehioui
Created: June 10th 2013

*/

#include "ShootTest.h"	

#include "ConversationVisitor.h"

namespace shoot
{
	DEFINE_OBJECT(ConversationVisitor);

	//! Reads/Writes struct properties from/to a stream
	void ConversationVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.SerializeArray("Phrases", &m_Phrases, PT_Struct);				
	}	

	//! visits a particular entity
	void ConversationVisitor::Visit(Entity* pTarget)
	{
		if(m_Phrases.GetSize())
		{
			for(u32 i=0; i<m_Phrases.GetSize(); ++i)
			{
				ConversationManager::Instance()->AddPhrase(m_Phrases[i]);
			}
			ConversationManager::Instance()->Start();
		}
	}
}

