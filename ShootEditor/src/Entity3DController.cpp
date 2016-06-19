/* 

Amine Rehioui
Created: May 14th 2010

*/

#include "ShootEditorCommon.h"

#include "Entity3DController.h"

#include "wxCommonProperties.h" // Vector3ToVariant

#include "Plane.h"

#include "CollisionUtils.h"

namespace shoot
{
	//! Constructor
	Entity3DController::Entity3DController()
		: m_SelectedAxes(Axe_None)
	{
		m_AxeHighlightColor.Set(0.0f, 1.0f, 1.0f);
		m_AxeSize = 1.0f;

		m_fTranslationStep = 0.2f;
		m_fRotationStep = 10.0f;
		m_fScaleStep = 0.05f;
		m_vIntersection = Vector3::Zero;
	}	

	//! destructor
	Entity3DController::~Entity3DController()
	{
	}

	//! Renders the controller elements
	void Entity3DController::Render()
	{
		if(m_Entity.IsValid())
		{
			m_EntityCenterTransformation = m_Entity->GetCenterTransformationMatrix();
			Matrix44 transform = Matrix44::Identity;
			transform.Translate(m_EntityCenterTransformation.GetTranslation());

			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, transform);

			// axe size should be proportional to the camera position and to the entity size
			Vector3 distToCamera = m_EntityCenterTransformation.GetTranslation() - EntityRenderer::Instance()->Get3DCamera()->GetPosition();
			m_fProportionalAxeSize = m_AxeSize * distToCamera.GetLength() / 8.0f;

			// X
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_X ? m_AxeHighlightColor : Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(m_fProportionalAxeSize, 0.0f, 0.0f));

			// Y
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_Y ? m_AxeHighlightColor : Color::Green);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(0.0f, m_fProportionalAxeSize, 0.0f));
			
			// Z
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_Z ? m_AxeHighlightColor : Color::Blue);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(0.0f, 0.0f, m_fProportionalAxeSize));

			// XY
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_XY ? m_AxeHighlightColor : Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(m_fProportionalAxeSize/2.0f, m_fProportionalAxeSize, 0.0f), Vector3::Create(m_fProportionalAxeSize, m_fProportionalAxeSize, 0.0f));
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_XY ? m_AxeHighlightColor : Color::Green);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(m_fProportionalAxeSize, m_fProportionalAxeSize/2.0f, 0.0f), Vector3::Create(m_fProportionalAxeSize, m_fProportionalAxeSize, 0.0f));			

			// XZ
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_XZ ? m_AxeHighlightColor : Color::Blue);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(m_fProportionalAxeSize, 0.0f, m_fProportionalAxeSize/2.0f), Vector3::Create(m_fProportionalAxeSize, 0.0f, m_fProportionalAxeSize));
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_XZ ? m_AxeHighlightColor : Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(m_fProportionalAxeSize/2.0f, 0.0f, m_fProportionalAxeSize), Vector3::Create(m_fProportionalAxeSize, 0.0f, m_fProportionalAxeSize));

			// ZY
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_ZY ? m_AxeHighlightColor : Color::Green);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, m_fProportionalAxeSize/2.0f, m_fProportionalAxeSize), Vector3::Create(0.0f, m_fProportionalAxeSize, m_fProportionalAxeSize));
			GraphicsDriver::Instance()->SetColor(m_SelectedAxes == Axe_ZY ? m_AxeHighlightColor : Color::Blue);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, m_fProportionalAxeSize, m_fProportionalAxeSize/2.0f), Vector3::Create(0.0f, m_fProportionalAxeSize, m_fProportionalAxeSize));
		}
	}

	//! Sets the 3D entity to control
	void Entity3DController::SetEntity(Entity3D* pEntity)
	{
		m_Entity = pEntity;
		if(!pEntity)
		{
			m_SelectedAxes = Axe_None;
		}
	}

	//! is called when the 3D picking ray has changed
	void Entity3DController::OnPickingRayMoved(const Vector3& vRayStart, const Vector3& vRayEnd)
	{
		m_SelectedAxes = Axe_None;
		Vector3 entityCenter = m_EntityCenterTransformation.GetTranslation();
		f32 thickness = m_fProportionalAxeSize/20.0f;
		AABBox3D boxes[] = 
		{
			AABBox3D(Vector3::Create(0.0f, -thickness, -thickness), Vector3::Create(m_fProportionalAxeSize, thickness, thickness)),
			AABBox3D(Vector3::Create(-thickness, 0.0f, -thickness), Vector3::Create(thickness, m_fProportionalAxeSize, thickness)),
			AABBox3D(Vector3::Create(-thickness, -thickness, 0.0f), Vector3::Create(thickness, thickness, m_fProportionalAxeSize)),
			AABBox3D(Vector3::Create(m_fProportionalAxeSize/2.0f, m_fProportionalAxeSize/2.0f, -thickness), Vector3::Create(m_fProportionalAxeSize, m_fProportionalAxeSize, thickness)), // XY
			AABBox3D(Vector3::Create(m_fProportionalAxeSize/2.0f, -thickness, m_fProportionalAxeSize/2.0f), Vector3::Create(m_fProportionalAxeSize, thickness, m_fProportionalAxeSize)), // XZ
			AABBox3D(Vector3::Create(-thickness, m_fProportionalAxeSize/2.0f, m_fProportionalAxeSize/2.0f), Vector3::Create(thickness, m_fProportionalAxeSize, m_fProportionalAxeSize)) // ZY
		};

		for(u32 i=Axe_X; i<=Axe_ZY; ++i)
		{
			boxes[i] = boxes[i] + entityCenter;
			if(CollisionUtils::AABBox3DIntersectsWithRay(boxes[i], vRayStart, vRayEnd, &m_vIntersection))
			{
				m_SelectedAxes = E_Axes(i);	
				break;
			}
		}
	}

	//! called when mouse is dragged
	void Entity3DController::OnMouseDragged(const Vector3& vRayStart, const Vector3& vRayDir)
	{
		if(m_SelectedAxes != Axe_None)
		{
			if(!m_Entity.IsValid())
			{
				m_SelectedAxes = Axe_None;
				return;
			}

			Vector3 entityCenter = m_EntityCenterTransformation.GetTranslation();
			Plane plane(Vector3::Zero, 0.0f);
			switch(m_SelectedAxes)
			{
			case Axe_X:
				plane = Plane(Vector3::Create(0.0f, 1.0f, 0.0f), entityCenter.Y);
				break;

			case Axe_Y:
				{
					Vector3 vNormal = Vector3::Create(vRayDir.X > vRayDir.Z ? 1.0f : 0.0f, 0.0f, vRayDir.Z > vRayDir.X ? 1.0f : 0.0f);
					plane = Plane(vNormal, vRayDir.X > vRayDir.Z ? entityCenter.X : entityCenter.Z);
				}
				break;

			case Axe_Z:
				plane = Plane(Vector3::Create(0.0f, 1.0f, 0.0f), entityCenter.Y);
				break;

			case Axe_XY:
				plane = Plane(Vector3::Create(0.0f, 0.0f, 1.0f), entityCenter.Z);
				break;

			case Axe_XZ:
				plane = Plane(Vector3::Create(0.0f, 1.0f, 0.0f), entityCenter.Y);
				break;

			case Axe_ZY:
				plane = Plane(Vector3::Create(1.0f, 0.0f, 0.0f), entityCenter.X);
				break;
			}
			Plane planes[] =
			{
				Plane(Vector3::Create(0.0f, 1.0f, 0.0f), entityCenter.Y),
				Plane(Vector3::Create(0.0f, 0.0f, 1.0f), entityCenter.Z),
				Plane(Vector3::Create(1.0f, 0.0f, 0.0f), entityCenter.X),
				Plane(Vector3::Create(1.0f, 0.0f, 0.0f), entityCenter.X),
				Plane(Vector3::Create(1.0f, 0.0f, 0.0f), entityCenter.X),
				Plane(Vector3::Create(1.0f, 0.0f, 0.0f), entityCenter.X),
			};		

			Vector3 newIntersection;
			if(plane.IntersectWithRay(vRayStart, vRayDir, &newIntersection))
			{				
				Vector3 delta = newIntersection-m_vIntersection;
				m_vIntersection = newIntersection;

				delta = Vector3::Create(m_SelectedAxes == Axe_X || m_SelectedAxes == Axe_XY || m_SelectedAxes == Axe_XZ ? delta.X : 0.0f,
										 m_SelectedAxes == Axe_Y || m_SelectedAxes == Axe_XY || m_SelectedAxes == Axe_ZY ? delta.Y : 0.0f,
										 m_SelectedAxes == Axe_Z || m_SelectedAxes == Axe_XZ || m_SelectedAxes == Axe_ZY ? delta.Z : 0.0f);

				switch(ShootEditor::Instance()->GetControlMode())
				{
				case ShootEditor::ControlMode_Translate:
					m_Entity->Translate(delta);

					// update position in property list
					ShootEditor::Instance()->GetObjectInspector()->UpdateProperty("Position", Vector3ToVariant(m_Entity->GetPosition())); 
					break;

				case ShootEditor::ControlMode_Rotate:
					m_Entity->Rotate(delta*m_fRotationStep);

					// update rotation in property list
					ShootEditor::Instance()->GetObjectInspector()->UpdateProperty("Rotation", Vector3ToVariant(m_Entity->GetRotation())); 
					break;

				case ShootEditor::ControlMode_Scale:
					m_Entity->Scale(delta);

					// update scale in property list
					ShootEditor::Instance()->GetObjectInspector()->UpdateProperty("Scale", Vector3ToVariant(m_Entity->GetScale())); 
					break;				
				}
			}
		}		
	}
}

