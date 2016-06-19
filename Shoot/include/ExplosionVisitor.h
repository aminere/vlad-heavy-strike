/* 

Amine Rehioui
Created: May 6th 2012

*/

#ifndef _EXPLOSION_VISITOR_H_INCLUDED_
#define _EXPLOSION_VISITOR_H_INCLUDED_

#include "Visitor.h"

#include "VertexBuffer.h"

#include "Timer.h"

#include "MeshResource.h"

namespace shoot
{
	//! ExplosionVisitor
	class ExplosionVisitor : public Visitor
	{
	public:

		//! explosion chunk
		struct Chunk : ISerializableStruct
		{
			//! constructor
			Chunk()
				: vCenter(Vector3::Zero)
				, vPosition(Vector3::Zero)
				, vRotation(Vector3::Zero)
				, vDirection(Vector3::Zero)
			{
				Reset();
			}

			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream);
			
			//! Resets the chunk
			inline void Reset()
			{
				vCenter = Vector3::Zero;
				vPosition = Vector3::Zero;
				vRotation = Vector3::Zero;
				vDirection = Vector3::Zero;
			}

			Vector3 vCenter;
			Vector3 vPosition;
			Vector3 vRotation;
			Vector3 vDirection;

			// properties
			Array<u32> aTriangleIndices;
		};

		//! ChunkGroup
		struct ChunkGroup : public ISerializableStruct
		{
			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream)
			{
				stream.SerializeArray("Chunks", &m_aChunks, PT_Struct);
			}

			Array<Chunk> m_aChunks;
		};

		DECLARE_OBJECT(ExplosionVisitor, Visitor);

		//! constructor
		ExplosionVisitor();

		//! destructor
		virtual ~ExplosionVisitor();

		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);

		//! visits a particular entity
		void Visit(Entity* pTarget);

		//! Leaves the visited entity
		void Leave();

		//! updates the visitor
		void Update();

		//! returns the target mesh
		MeshResource* GetMesh() const { return m_Mesh; }

		//! sets the intensity
		inline void SetDefaultIntensity(f32 fIntensity) { m_fDefaultIntensity = fIntensity; }

		//! sets the default duration
		inline void SetDefaultDuration(f32 fDuration) { m_fDefaultDuration = fDuration; }

		//! returns the duration
		inline f32 GetDuration() const { return m_fDuration; }

#ifdef SHOOT_EDITOR
		//! returns the explosion chunk groups
		Array<ChunkGroup>& GetChunkGroups() { return m_aChunkGroups; }
#endif // SHOOT_EDITOR

	private:

		std::vector< Reference<VertexBuffer> > m_VertexBuffers;
		std::vector< Reference<VertexBuffer> > m_OriginalVertexBuffers;
		std::vector< Reference<Material> > m_OriginalMaterials;
		Timer m_Timer;
		f32 m_fDefaultDuration;
		f32 m_fDefaultIntensity;

		// properties
		Array<ChunkGroup> m_aChunkGroups;
		Reference<MeshResource> m_Mesh;
		f32 m_fDuration;
		f32 m_fIntensity;
		bool m_bDepthTest;
	};
}

#endif // _EXPLOSION_VISITOR_H_INCLUDED_


