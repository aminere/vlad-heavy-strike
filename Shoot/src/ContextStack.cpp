/* 

Amine Rehioui
Created: April 11th 2010

*/

#include "Shoot.h"

#include "ContextStack.h"

namespace shoot
{
	DEFINE_OBJECT(ContextStack);

	//! Constructor
	ContextStack::ContextStack()
	{
	}

	//! Destructor
	ContextStack::~ContextStack()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void ContextStack::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void ContextStack::Init()
	{
		super::Init();
	}

	//! called during the update of the entity
	void ContextStack::Update()
	{
		for(s32 i=s32(GetChildCount())-1; i >= 0; --i)
		{
			Context* pContext = static_cast<Context*>(GetChild(i));
			if(pContext->IsEnabled())
			{
				pContext->Update();

				if(pContext->CanBlockUpdate())
				{
					break;
				}
			}
		}
	}

	//! registers the entity and its children for rendering
	void ContextStack::RegisterForRendering()
	{
		for(u32 i=0; i<GetChildCount(); ++i)
		{
			GetChild(i)->RegisterForRendering();
		}
	}

	//! adds a child to the entity
	void ContextStack::AddChild(Entity* pChild, bool bImmediate /*= false*/)
	{
		SHOOT_ASSERT(pChild->IsA(Context::TypeID), "ContextStack::AddChild: child is not of type Context");
		super::AddChild(pChild, bImmediate);
	}
}

