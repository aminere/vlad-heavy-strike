/* 

Amine Rehioui
Created: August 14th 2011

*/

#include "Shoot.h"

#include "CameraPathElement.h"

#include "EditorEvents.h"

namespace shoot
{
	DEFINE_OBJECT(CameraPathElement);

	//! serializes the entity to/from a PropertyStream
	void CameraPathElement::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Vec3, "CameraLookAt", &m_vCameraLookAt);

#ifdef SHOOT_EDITOR
		std::string setCameraInfoEvent(EditorEvents::SetCameraPathElementInfoEvent::GetStaticClassName());
		stream.Serialize(PT_Event, "SetCameraInfo", &setCameraInfoEvent);
#endif // SHOOT_EDITOR
	}
}

