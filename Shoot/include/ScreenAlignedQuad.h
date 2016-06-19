/* 

Amine Rehioui
Created: June 25th 2011

*/

#ifndef _SCREEN_ALIGNED_QUAD_H_INCLUDED_
#define _SCREEN_ALIGNED_QUAD_H_INCLUDED_

namespace shoot
{
	//! Screen Aligned Quad
	class ScreenAlignedQuad
	{
	public:

		//! sets the texture
		void SetTexture(Texture* pTexture) { m_Texture.SetObject(pTexture); }

		//! renders the quad
		void Render();

	private:

		Reference<Texture> m_Texture;
	};
}

#endif // _SCREEN_ALIGNED_QUAD_H_INCLUDED_

