/* 

Amine Rehioui
Created: April 21th 2010

*/

#ifndef _TEXT_H_INCLUDED_
#define _TEXT_H_INCLUDED_

#include "Font.h"

namespace shoot
{
	//! Text entity
	class Text : public Entity2D				 
	{
	public:

		//! Alignement
		enum E_Alignement
		{
			A_Left,
			A_Center,
			A_Right
		};

		//! Macro to provide type information
		DECLARE_OBJECT(Text, Entity2D);

		//! constructor
		Text();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();
			
		//! registers the entity and its children for rendering		
		virtual void RegisterForRendering();

		//! Get the local axis-aligned bounding box
		virtual AABBox2D GetBoundingBox() const;

		//! Reloads the font given a font path
		void SetFont(std::string strPath);

		//! sets the text
		void SetText(const char* strText);

		//! sets the color
		void SetColor(const Color& color);

		//! returns the text
		const std::string& GetText() const { return m_strText; }

		//! sets the desired vertex count used for rendering
		inline void SetDesiredVertexCount(s32 count) { m_DesiredVertexCount = count; }

		//! sets the text area size
		inline void SetAreaSize(const Size& size) { m_Size = size; }

		//! sets the line spacing
		inline void SetLineSpacing(u32 spacing) { m_LineSpacing = spacing; }

		//! pre-rendering
		void PreRender();

	private:		

		//! updates the geometry
		void UpdateGeometry(const std::string& text, const Color& color);

		bool m_bDirty;
		s32 m_DesiredVertexCount;

		// properties
		std::string m_strText;
		Reference<Font> m_Font;		
		Size m_Size;
		E_Alignement m_eAlignement;
		u32 m_LineSpacing;
		Color m_Color;
	};
}

#endif // _TEXT_H_INCLUDED_

