/* 

Amine Rehioui
Created: October 17th 2011

*/

#include "Shoot.h"

#include "StatsManager.h"

#include "MemoryManager.h"

#include "Text.h"

#include <stdio.h> // sprintf

#include "InputManager.h"

#include "FileSystem.h"

namespace shoot
{
	//! constructor
	StatsManager::StatsManager()
		: m_bEnabled(false)
	{
		m_FPS = snew Text();
		m_FPS->SetFont("common/fixed.xml");
		m_FPS->SetPosition(Vector2(0.0f, 16.0f*0));
		m_MemoryUsed = snew Text();
		m_MemoryUsed->SetFont("common/fixed.xml");
		m_MemoryUsed->SetPosition(Vector2(0.0f, 16.0f*1));
		m_NumDrawCalls = snew Text();
		m_NumDrawCalls->SetFont("common/fixed.xml");
		m_NumDrawCalls->SetPosition(Vector2(0.0f, 16.0f*2));
		m_FileSystemInfo = snew Text();
		m_FileSystemInfo->SetAreaSize(Size(480, 32));
		m_FileSystemInfo->SetLineSpacing(16);
		m_FileSystemInfo->SetFont("common/fixed.xml");
		m_FileSystemInfo->SetPosition(Vector2(0.0f, 16.0f*3));
		m_QuadTreeInfo = snew Text();
		m_QuadTreeInfo->SetFont("common/fixed.xml");
		m_QuadTreeInfo->SetPosition(Vector2(0.0f, 16.0f*5));
	}

	//! destructor
	StatsManager::~StatsManager()
	{
	}

	//! update
	void StatsManager::Update()
	{
		if(InputManager::Instance()->IsKeyJustReleased(InputManager::KT_L))
		{
			m_bEnabled = !m_bEnabled;
		}

		char buf[512];
		sprintf(buf, "FPS: %.2f", Engine::Instance()->GetFPS());
		m_FPS->SetText(buf);

		sprintf(buf, "RAM: %.2f KB", f32(MemoryManager::GetUsedMemory())/1024.0f);
		m_MemoryUsed->SetText(buf);

		if(m_bEnabled)
		{
			Engine* pEngine = Engine::Instance();
			QuadTree* pQuadTree = pEngine->GetNumContexts() ? pEngine->GetContext(0)->GetQuadTree() : NULL;
			if(pQuadTree)
			{
				sprintf(buf, "Nodes drawn %d/%d", pQuadTree->GetNodesDrawn(), pQuadTree->GetNodeCount());
				m_QuadTreeInfo->SetText(buf);
			}
			else
			{
				m_QuadTreeInfo->SetText("");
			}

			EntityRenderer::Stats& renderingStats = EntityRenderer::Instance()->GetStats();
			sprintf(buf, "Mat %d, Vtx %d, Tri %d, Draw %d", renderingStats.NumMaterials, renderingStats.NumVertices, renderingStats.NumTriangles, renderingStats.NumDrawCalls);
			m_NumDrawCalls->SetText(buf);

			std::vector<FileSystem::PartitionInfo>& partitions = FileSystem::Instance()->GetPartitions();
			sprintf(buf, "(%d) ", partitions.size());
			for(u32 i=0; i<partitions.size(); ++i)
			{
				char strSize[32];
				sprintf(strSize, "%d ", partitions[i].pData ? partitions[i].dataSize/1024 : 0);			
				strcat(buf, strSize);
			}
			m_FileSystemInfo->SetText(buf);
		}
	}

	//! registers for rendering
	void StatsManager::RegisterForRendering()
	{
#define REGISTER_TEXT(x) if(x->GetText().length()) { EntityRenderer::Instance()->RegisterDebugEntity(x); }
		
		if(m_bEnabled)
		{
			REGISTER_TEXT(m_FPS);
			REGISTER_TEXT(m_MemoryUsed);
			REGISTER_TEXT(m_NumDrawCalls);
			REGISTER_TEXT(m_FileSystemInfo);
			REGISTER_TEXT(m_QuadTreeInfo);
		}		
	}	
}
