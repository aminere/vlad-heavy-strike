/* 

Amine Rehioui
Created: June 15th 2013

*/

#ifndef _FADE_RENDERER_H_INCLUDED_
#define _FADE_RENDERER_H_INCLUDED_

namespace shoot
{
	//! FadeRenderer
	class FadeRenderer : public Object
	{
		DECLARE_OBJECT(FadeRenderer, Object);

	public:

		//! constructor
		FadeRenderer();

		//! destructor
		virtual ~FadeRenderer();

		//! renders the fade effect
		void Render();

		//! resizes the fade effect
		void Resize();

		//! returns the fade color
		inline const Color& GetColor() const { return m_Color; }

		//! sets the fade color
		void SetColor(const Color& color);

		//! starts a fade effect
		void StartFade(const Color& srcColor, const Color& destColor, f32 fDuration); 

		//! returns the instance
		inline static FadeRenderer* Instance()
		{
			return ms_pInstance;
		}

	private:

		Reference<VertexBuffer> m_FadeQuad;
		Reference<Material> m_Material;
		Color m_Color;

		bool m_bFadeInProgress;
		f32 m_fFadeDuration;
		f32 m_fFadeTimer;
		Color m_FadeSrcColor;
		Color m_FadeDestColor;

		static FadeRenderer* ms_pInstance;
	};
}

#endif // _FADE_RENDERER_H_INCLUDED_

