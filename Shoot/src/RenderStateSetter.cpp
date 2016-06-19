/* 

Amine Rehioui
Created: April 7th 2012

*/

#include "Shoot.h"

#include "RenderStateSetter.h"

#include "GraphicExtensionHandler.h"

namespace shoot
{
	DEFINE_OBJECT(RenderStateSetter);

	//! begins rendering with this setter
	void PointSpriteStateSetter::Begin()
	{
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_PointSprite, true);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_PointSize, m_fPointSize*(GraphicsDriver::Instance()->GetViewPort().Size().Y/600.0f));
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_MinPointSize, 1.0f);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_MaxPointSize, 9999.0f);
		// dist_atten(d) = 1/(a + b*d + c*d^2)
		const f32 attenuation[3] = { 0.0f,
									 0.01f * (EntityRenderer::Instance()->Get3DCamera()->GetFOV()/45.0f),
									 0.0f };
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_PointDistanceAttenuation, attenuation);
	}

	//! ends rendering with this setter
	void PointSpriteStateSetter::End()
	{
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_PointSprite, false);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_PointSize, 1.0f);
	}

	//! begins rendering with this setter
	void SkyBoxStateSetter::Begin()
	{
		Camera* pCamera = EntityRenderer::Instance()->Get3DCamera();
		Vector3 direction = pCamera->GetLookAt() - pCamera->GetPosition();

		Matrix44 viewMatrix;
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_CubeMap))
		{
			// convert from shoot's convention XRight/YFront/ZUp to cubemap convention XRight/YUp/ZFront
			viewMatrix.BuildLookAtLH(Vector3::Zero,
									 Vector3::Create(direction.X, direction.Z, -direction.Y),
									 Vector3::Create(0.0f, 1.0f, 0.0f));
		}
		else
		{
			// convert from shoot's convention XRight/YFront/ZUp to cubemap convention XRight/YUp/ZFront
			viewMatrix.BuildLookAtLH(Vector3::Zero,
									 direction,
									 pCamera->GetUpVector());
		}
		
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, viewMatrix);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthWriting, false);
	}

	//! end rendering with this setter
	void SkyBoxStateSetter::End()
	{
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthWriting, true);
	}
}

