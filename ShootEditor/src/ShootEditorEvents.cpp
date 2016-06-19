/* 

Amine Rehioui
Created: August 7th 2011

*/

#include "ShootEditorCommon.h"
#include "ShootEditorEvents.h"
#include "EditorEvents.h"
#include "EventManager.h"

#include "CameraManager.h"

#include "CameraPathElement.h" // HandleSetCameraPathElementInfoEvent

namespace shoot
{
	//! constructor
	ShootEditorEventsHandler::ShootEditorEventsHandler()
	{
		EventManager::Instance()->RegisterListener<EditorEvents::SetCameraPathElementInfoEvent>(this, &HandleSetCameraPathElementInfoEvent);
	}

	//! destructor
	ShootEditorEventsHandler::~ShootEditorEventsHandler()
	{
		EventManager::Instance()->UnregisterListener<EditorEvents::SetCameraPathElementInfoEvent>(this);
	}

	//! handles SetCameraPathElementInfoEvent
	void ShootEditorEventsHandler::HandleSetCameraPathElementInfoEvent(void* pListener, Event* pEvent)
	{
		CameraPathElement* pElement = DYNAMIC_CAST(ShootEditor::Instance()->GetSelectedEntity(), CameraPathElement);
		SHOOT_ASSERT(pElement, "HandleSetCameraPathElementInfoEvent entity not of type CameraPathElement");

		CameraManager::Instance()->SetCameraPathElementInfo(true);

		// switch to editor camera
		// move camera to path element location
		if(ViewPort* pViewPort = ShootEditor::Instance()->GetViewPort())
		{
			Camera* pEditorCamera = pViewPort->Get3DCamera();
			pEditorCamera->SetPosition(pElement->GetTransformationMatrix().GetTranslation());
			pEditorCamera->SetLookAt(pElement->GetCameraLookAt());
			EntityRenderer::Instance()->Set3DCamera(pEditorCamera);
			ShootEditor::Instance()->UpdateCameraSelectors();
		}		
	}
}
