/* 

Amine Rehioui
Created: December 10th 2010

*/

#ifndef _VERTEX_BUFFER_H_INCLUDED_
#define _VERTEX_BUFFER_H_INCLUDED_

#include "RenderStateSetter.h"

namespace shoot
{
	//! vertex buffer
	class VertexBuffer : public Object, public GraphicObject
	{
		DECLARE_OBJECT(VertexBuffer, Object);

	public:

		//! constructor
		VertexBuffer();

		//! destructor
		virtual ~VertexBuffer();

		//! begins rendering using this vertex buffer
		virtual void Begin();

		//! default render method
		virtual void Draw() { }

		//! instanced render method
		virtual void Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial) { }

		//! ends rendering by this vertex buffer
		virtual void End();
		
		//! returns a pointer to the vertices
		inline Vertex3D* GetVertices() const { return m_pVertices; }

		//! returns a pointer to the vertices
		inline Vertex3D* GetVertices() { return m_pVertices; }

		//! sets the pointers to the vertices
		void SetVertices(Vertex3D* pVertices, u32 numVertices);
		
		//! returns the number of vertices
		inline u32 GetNumVertices() const { return m_NumVertices; }

		//! returns the max vertices
		inline u32 GetMaxVertices() const { return m_MaxVertices; }

		//! sets the number of vertices
		inline void SetNumVertices(u32 numVertices)
		{
			SHOOT_ASSERT(numVertices <= m_MaxVertices, "VertexBuffer MaxVertices reached");
			m_NumVertices = numVertices;
		}

		//! sets the max number of vertices
		inline void SetMaxVertices(u32 maxVertices)
		{
			SHOOT_ASSERT(maxVertices <= m_AllocatedVertices, "VertexBuffer AllocatedVertices reached");
			m_MaxVertices = maxVertices;
		}

		//! returns the vertex flags
		inline u32 GetVertexFlags() const { return m_Flags; }

		//! sets the vertex flags
		inline void SetVertexFlag(Vertex3D::E_VertexFlag eFlag) { m_Flags |= eFlag; }

		//! sets the vertex flags
		inline void SetVertexFlags(u32 flags) { m_Flags = flags; }

		//! returns a pointer to the indices
		inline u16* GetIndices() const { return m_pIndices; }

		//! sets the pointers to the indices
		void SetIndices(u16* pIndices, u32 numIndices);
		
		//! returns the number of indices
		inline u32 GetNumIndices() const { return m_NumIndices; }

		//! sets the number of indices
		inline void SetNumIndices(u32 numIndices)
		{
			SHOOT_ASSERT(numIndices < m_MaxIndices, "VertexBuffer m_MaxIndices reached");
			m_NumIndices = numIndices;
		}

		//! sets the primitive type
		inline void SetPrimitiveType(GraphicsDriver::E_PrimitiveType eType) { m_ePrimitiveType = eType; }

		//! returns the primitive type
		inline GraphicsDriver::E_PrimitiveType GetPrimitiveType() const { return m_ePrimitiveType; }

		//! sets the render state setter
		inline void SetRenderStateSetter(RenderStateSetter* pSetter) { m_RenderStateSetter.SetObject(pSetter); }

		//! gets the render state setter
		inline RenderStateSetter* GetRenderStateSetter() { return m_RenderStateSetter.Get(); }

		//! sets the dynamic flag
		inline void SetDynamic(bool bDynamic) { m_bDynamic = bDynamic; }

		//! gets the dynamic flag
		inline bool GetDynamic() const { return m_bDynamic; }

		//! sets the dirty flag
		inline void SetDirty(bool bDirty) { m_bDirty = bDirty; }

		//! updates the vertex buffer data
		virtual void UpdateData() { }

		//! set apply world transform status
		inline void SetApplyWorldTransforms(bool bApply) { m_bApplyWorldTransforms = bApply; }

	protected:

		Vertex3D* m_pVertices;
		u32 m_MaxVertices;
		u32 m_NumVertices;
		u32 m_AllocatedVertices;
		u32 m_Flags;
		u16* m_pIndices;
		u32 m_MaxIndices;
		u32 m_NumIndices;
		GraphicsDriver::E_PrimitiveType m_ePrimitiveType;
		Reference<RenderStateSetter> m_RenderStateSetter;
		bool m_bDynamic;
		bool m_bDirty;
		bool m_bApplyWorldTransforms;
	};
}

#endif // _VERTEX_BUFFER_H_INCLUDED_

