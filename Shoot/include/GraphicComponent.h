/* 

Amine Rehioui
Created: February 23rd 2013

*/

#ifndef _GRAPHIC_COMPONENT_H_INCLUDED_
#define _GRAPHIC_COMPONENT_H_INCLUDED_

#include "Material.h"

#include "VertexBuffer.h"

namespace shoot
{
	//! GraphicComponent class
	class GraphicComponent : public Component
	{
		DECLARE_OBJECT(GraphicComponent, Component);

	public:

		//! rendering pass types
		enum E_RenderingPass
		{
			RP_SkyBox,
			RP_3D,
			RP_Transparent3D,
			RP_2D,
			RP_Transparent2D,
			RP_2DPostFade,
			RP_None,
			RP_Count
		};

		//! constructor
		GraphicComponent();

		//! destructor
		virtual ~GraphicComponent();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! sets the vertex buffer
		void SetVertexBuffer(VertexBuffer* pVertexBuffer) { m_VertexBuffer.SetObject(pVertexBuffer); }

		//! returns the vertex buffer
		VertexBuffer* GetVertexBuffer() { return m_VertexBuffer.Get(); }

		//! sets the material
		void SetMaterial(Material* pMaterial) { m_Material.SetObject(pMaterial); }

		//! returns the material
		Material* GetMaterial() { return m_Material.Get(); }

		//! returns the rendering priority
		inline u32 GetRenderingPriority() const { return m_RenderingPriority; }

		//! sets the 2D rendering priority
		inline void SetRenderingPriority(u32 priority) { m_RenderingPriority = priority; }

		//! returns the rendering pass
		E_RenderingPass GetRenderingPass() const { return m_eRenderingPass; }

		//! sets the rendering pass
		inline void SetRenderingPass(E_RenderingPass ePass) { m_eRenderingPass = ePass; }

	private:

		Reference<VertexBuffer> m_VertexBuffer;
		Reference<Material> m_Material;
		u32 m_RenderingPriority;
		E_RenderingPass m_eRenderingPass;
	};
}

#endif // _GRAPHIC_COMPONENT_H_INCLUDED_

