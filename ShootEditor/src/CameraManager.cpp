/* 

Amine Rehioui
Created: August 19th 2011

*/

#include "ShootEditorCommon.h"

#include "CameraManager.h"

#include "CameraPathElement.h"

#include "EditorSettings.h"

#include "wxCommonProperties.h"

namespace shoot
{
	//! static variables initialization
	f32 CameraManager::m_CameraIncrStepTime = 0.6f;

	//! constructor
	CameraManager::CameraManager()
		: m_bGameCameraControl(false)
		, m_bSetCameraPathElementInfo(false)
		, m_eMode(M_Spectate)
		, m_f3DStrafeStep(EditorSettings::Instance()->f3DStrafeStep)
		, m_f3DZoomStep(EditorSettings::Instance()->f3DZoomStep)		
	{
	}

	//! destructor
	CameraManager::~CameraManager()
	{
	}

	//! Strafe the camera
	void CameraManager::StrafeCamera(Camera* pCamera, const Vector2& vStrafeDir)
	{
		switch(pCamera->GetType())
		{
		case Camera::Type_Perspective:
		case Camera::Type_Ortho3D:
			{
				if(m_eMode == M_Spectate)
				{
					Vector3 lookAt = pCamera->GetLookAt() - pCamera->GetPosition();			
					Vector3 lateralStrafeVect = lookAt.CrossProduct(pCamera->GetUpVector());
					Vector3 verticalStrafeVect = lateralStrafeVect.CrossProduct(lookAt);
					lateralStrafeVect.Normalize();
					verticalStrafeVect.Normalize();
					lateralStrafeVect = lateralStrafeVect * -vStrafeDir.X;
					verticalStrafeVect = verticalStrafeVect * vStrafeDir.Y;
					Vector3 strafeVect = lateralStrafeVect + verticalStrafeVect;		
					strafeVect = strafeVect * m_f3DStrafeStep;
					pCamera->Translate(strafeVect);
					pCamera->SetLookAt(pCamera->GetLookAt() + strafeVect);

					if(m_bSetCameraPathElementInfo)
					{
						CameraPathElement* pElement = static_cast<CameraPathElement*>(ShootEditor::Instance()->GetSelectedEntity());
						pElement->SetAbsolutePosition(pCamera->GetPosition());
						pElement->SetCameraLookAt(pCamera->GetLookAt());
					}
				}
			}
			break;

		case Camera::Type_Ortho2D:
			pCamera->Translate(Vector3::Create(vStrafeDir.X, vStrafeDir.Y, 0.0f));
			break;
		}
	}	

	//! Zoom the 3D camera
	void CameraManager::ZoomCamera(Camera* pCamera, f32 fDir)
	{
		switch(pCamera->GetType())
		{
		case Camera::Type_Perspective:
		case Camera::Type_Ortho3D:
			{
				switch(m_eMode)
				{
				case M_Spectate:
					{
						Vector3 vDir = pCamera->GetLookAt() - pCamera->GetPosition();
						vDir.Normalize();
						vDir = vDir * m_f3DZoomStep * fDir;
						pCamera->Translate(vDir);
						pCamera->SetLookAt(pCamera->GetLookAt() + vDir);

						if(m_bSetCameraPathElementInfo)
						{
							CameraPathElement* pElement = static_cast<CameraPathElement*>(ShootEditor::Instance()->GetSelectedEntity());
							pElement->SetAbsolutePosition(pCamera->GetPosition());
							pElement->SetCameraLookAt(pCamera->GetLookAt());
						}
					}
					break;

				case M_Inspect:
					{
						Vector3 vDir = pCamera->GetLookAt() - pCamera->GetPosition();
						vDir.Normalize();
						vDir = vDir * m_f3DZoomStep * fDir;
						pCamera->Translate(vDir);
					}
					break;
				}
			}
			break;

		case Camera::Type_Ortho2D:			
			{
				f32 fDeltaZoom = EditorSettings::Instance()->f2DZoomStep * fDir;
				pCamera->SetScale(pCamera->GetScale() + Vector3::Create(fDeltaZoom, fDeltaZoom, 0.0f));
			}
			break;
		}
	}

	//! Rotate the camera
	void CameraManager::RotateCamera(Camera* pCamera, const Vector2& vRotation)
	{
		Vector3 lookAt = pCamera->GetLookAt() - pCamera->GetPosition();
		Vector3 lateralAxeRotation = lookAt.CrossProduct(pCamera->GetUpVector());
		Vector3 verticalAxeRotation = lateralAxeRotation.CrossProduct(lookAt);
		lateralAxeRotation.Normalize();
		verticalAxeRotation.Normalize();
		lateralAxeRotation = lateralAxeRotation * vRotation.Y * Math::DegToRadFactor * EditorSettings::Instance()->f3DLookStep;
		verticalAxeRotation = verticalAxeRotation * vRotation.X * Math::DegToRadFactor * EditorSettings::Instance()->f3DLookStep;
		Vector3 v3DRot = lateralAxeRotation + verticalAxeRotation;

		switch(m_eMode)
		{
		case M_Spectate:
			{
				// rotate the camera look at point with respect to the camera position
				Vector3 localPos = pCamera->GetLookAt() - pCamera->GetPosition();
				localPos.Rotate(v3DRot);
				pCamera->SetLookAt(pCamera->GetPosition() + localPos);	

				if(m_bSetCameraPathElementInfo)
				{
					CameraPathElement* pElement = static_cast<CameraPathElement*>(ShootEditor::Instance()->GetSelectedEntity());
					pElement->SetCameraLookAt(pCamera->GetLookAt());
				}
			}
			break;

		case M_Inspect:
			{
				// rotate the camera position with respect to the look at point
				Vector3 localPos = pCamera->GetPosition() - pCamera->GetLookAt();
				localPos.Rotate(v3DRot);
				pCamera->SetPosition(pCamera->GetLookAt() + localPos);
			}
			break;
		}
	}

	//! updates camera control steps
	void CameraManager::UpdateControlSteps()
	{
		f32 fDeltaTime = f32(m_CameraStepTimer.Time())/1000.0f;		
		if(fDeltaTime > m_CameraIncrStepTime)
		{
			m_f3DStrafeStep = EditorSettings::Instance()->f3DStrafeStep;
			m_f3DZoomStep = EditorSettings::Instance()->f3DZoomStep;
		}
		else
		{
			m_f3DStrafeStep *= EditorSettings::Instance()->f3DStrafeStepIncrFactor;
			m_f3DZoomStep *= EditorSettings::Instance()->f3DZoomStepIncrFactor;
		}
		m_CameraStepTimer.Start();
	}

	//! check property overrides
	void CameraManager::CheckPropertyOverrides(Camera* pCamera)
	{
		if(pCamera == ShootEditor::Instance()->GetSelectedEntity())
		{
			ShootEditor::Instance()->GetObjectInspector()->UpdateProperty("Position", Vector3ToVariant(pCamera->GetPosition()));
			ShootEditor::Instance()->GetObjectInspector()->UpdateProperty("LookAt", Vector3ToVariant(pCamera->GetLookAt()));
		}
		else
		{
			PropertyStream stream(SM_Write);
			pCamera->Serialize(stream);

			if(!pCamera->GetTemplatePath().empty() 
			&& !pCamera->GetOverridenProperties().GetProperty("Position"))
			{
				pCamera->GetOverridenProperties().AddProperty(stream.GetProperty("Position")->Copy());
			}

			if(!pCamera->GetTemplatePath().empty() 
			&& !pCamera->GetOverridenProperties().GetProperty("LookAt"))
			{
				pCamera->GetOverridenProperties().AddProperty(stream.GetProperty("LookAt")->Copy());
			}
		}
	}

	//! sets SetCameraPathElementInfo status
	void CameraManager::SetCameraPathElementInfo(bool bEnabled)
	{ 
		m_bSetCameraPathElementInfo = bEnabled;

		if(bEnabled)
		{
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("SetCameraPathElementInfo", 3);
		}
		else
		{
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("", 3);
		}
	}
}
