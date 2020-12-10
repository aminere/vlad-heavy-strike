/* 

Amine Rehioui
Created: February 28th 2010

*/

#include "Shoot.h"

#include "Context.h"

#include "Thread.h"

namespace shoot
{
	DEFINE_OBJECT(Context);

	//! constructor
	Context::Context()
		: m_pLoadingThread(NULL)
		, m_bLoadingThreadFinished(false)
		// properties
		, m_bBlockUpdate(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Context::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.SerializeReference("QuadTree", &m_QuadTree);
		stream.Serialize(PT_Bool, "BlockUpdate", &m_bBlockUpdate);
	}

	//! called during the initialization of the entity
	void Context::Init()
	{
		m_CollisionManager = snew CollisionManager();
		m_CollisionManager->SetContext(this);

		super::Init();

		if(m_QuadTree.IsValid())
		{
			m_QuadTree->Build(this);
		}
	}

	//! called during the update of the entity
	void Context::Update()
	{
		if(IsLoading())
		{
			if(m_bLoadingThreadFinished)
			{
				sdelete(m_pLoadingThread);
				m_bLoadingThreadFinished = false;
			}
			return;
		}

		m_CollisionManager->Update();

		if(m_QuadTree.IsValid())
		{
			m_QuadTree->Update();
		}
		else
		{
			for(u32 i=0; i<GetChildCount(); ++i)
			{
				Update(GetChild(i));
			}
		}
	}

	//! registers the entity and its children for rendering
	void Context::RegisterForRendering()
	{
		if(IsLoading())
		{
			return;
		}

		if(m_QuadTree.IsValid())
		{
			m_QuadTree->RegisterForRendering();
		}
		else
		{
			for(u32 i=0; i<GetChildCount(); ++i)
			{
				RegisterForRendering(GetChild(i));
			}
		}
	}
	
	//! recursively update children
	void Context::Update(Entity* pEntity)
	{
		if(pEntity->IsEnabled())
		{
			pEntity->Update();

			for(u32 i=0; i<pEntity->GetComponentCount(); ++i)
			{
				pEntity->GetComponent(i)->Update();
			}

			for(u32 i=0; i<pEntity->GetChildCount(); ++i)
			{
				Update(pEntity->GetChild(i));
			}
		}
	}

	//! recursivly register entities for rendering
	void Context::RegisterForRendering(Entity* pEntity)
	{
		if(pEntity->IsVisible() && pEntity->IsInitialized())
		{
			pEntity->RegisterForRendering();

			for(u32 i=0; i<pEntity->GetChildCount(); ++i)
			{
				RegisterForRendering(pEntity->GetChild(i));
			}
		}
	}

	//! switches to a new context
	void Context::Switch(const char* strPath)
	{
		LoadFromXML(strPath);
		ObjectManager::Instance()->SetTemplatePath(this, strPath);
		Init();
	}

	//! pops the context
	void Context::Pop()
	{
		GetParent()->RemoveChild(this, true);
	}

	//! returns true if the context is on top of the context stack
	bool Context::IsTopContext() const
	{
		auto topContextIndex = Engine::Instance()->GetNumContexts()-1;
		return this == Engine::Instance()->GetContext(topContextIndex);
	}
}

