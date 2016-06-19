/* 

Amine Rehioui
Created: June 15th 2013

*/

#include "Shoot.h"

#include "FadeRenderer.h"
#include "GeometryProvider.h"
#include "MaterialProvider.h"

namespace shoot
{
	DEFINE_OBJECT(FadeRenderer);

	//! static vars
	FadeRenderer* FadeRenderer::ms_pInstance = NULL;

	//! constructor
	FadeRenderer::FadeRenderer()
		: m_Color(Color::Zero)
		, m_bFadeInProgress(false)
	{
		SHOOT_ASSERT(!ms_pInstance, "Multiple FadeRenderer instances detected");
		ms_pInstance = this;

		Resize();

		Material::CreationInfo materialInfo;
		Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
		pInfo->m_Path = "common/tex/WhiteBG.png";
		pInfo->m_bMipMaps = false;		
		materialInfo.m_TextureInfos.Add(pInfo);
		materialInfo.SetFlag(Material::MF_AlphaBlending, true);
		materialInfo.SetFlag(Material::MF_DepthTest, false);
		m_Material = MaterialProvider::Instance()->GetMaterial(materialInfo);
	}

	//! destructor
	FadeRenderer::~FadeRenderer()
	{
		ms_pInstance = NULL;
	}

	//! renders the fade effect
	void FadeRenderer::Render()
	{
		if(m_bFadeInProgress)
		{
			if(m_fFadeTimer < m_fFadeDuration)
			{
				m_Color = Math::Lerp(m_FadeSrcColor, m_FadeDestColor, m_fFadeTimer/m_fFadeDuration);
				m_fFadeTimer += g_fDeltaTime;
			}
			else
			{
				m_Color = m_FadeDestColor;
				m_bFadeInProgress = false;
			}
		}

		Matrix44 previousViewMatrix = GraphicsDriver::Instance()->GetTransform(GraphicsDriver::TS_View);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, Matrix44::Identity);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
		m_Material->SetColor(m_Color);
		m_Material->Begin();
			m_FadeQuad->Begin();
				m_FadeQuad->Draw(std::vector<Matrix44>(), std::vector<Matrix44>(), m_Material);
			m_FadeQuad->End();
		m_Material->End();
#ifdef DX11
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, previousViewMatrix.GetTranspose());
#else
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, previousViewMatrix);
#endif
	}

	//! resizes the fade effect
	void FadeRenderer::Resize()
	{
		Vector2 vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
		m_FadeQuad = GeometryProvider::Instance()->Get2DQuad(vScreenSize, Vector2(1.0f, 1.0f));
		m_FadeQuad->SetApplyWorldTransforms(false);
	}

	//! sets the fade color
	void FadeRenderer::SetColor(const Color& color)
	{
		m_Color = color;
		m_bFadeInProgress = false;
	}

	//! starts a fade effect
	void FadeRenderer::StartFade(const Color& srcColor, const Color& destColor, f32 fDuration)
	{		
		m_FadeSrcColor = srcColor;
		m_FadeDestColor = destColor;		
		m_fFadeDuration = fDuration;
		m_fFadeTimer = 0.0f;
		m_bFadeInProgress = true;
	}
}

