/* 

Amine Rehioui
Created: December 18th 2010

*/

#include "Types.h"

#include "Player.h"

#include "InputManager.h"

#include "MathUtils.h"

#include "Printer.h"

#include "GraphicsDriver.h"

// collision testing
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Triangle.h"

//! Constructor
Player::Player()
: m_vVelocity(0.0f, 0.0f, 0.0f)
, m_vSlidingDirection(0.0f, 0.0f, 0.0f)
, m_iCurrentCollidingTriangle(-1)
// properties
, m_fGravity(-0.1f)
, m_fMaxSpeed(1.0f)
, m_fAcceleration(0.02f)
, m_fRadius(0.5f)
{
}

//! serializes the entity to/from a PropertyStream
void Player::Serialize(shoot::PropertyStream& stream)
{
	super::Serialize(stream);

	stream.Serialize(shoot::PT_Float, "Gravity", &m_fGravity);
	stream.Serialize(shoot::PT_Float, "MaxSpeed", &m_fMaxSpeed);
	stream.Serialize(shoot::PT_Float, "Acceleration", &m_fAcceleration);
	stream.Serialize(shoot::PT_Float, "Radius", &m_fRadius);
}

//! called during the initialization of the entity
void Player::Init()
{
	super::Init();

	m_pEnvironment = static_cast<shoot::MeshEntity*>(this->GetRoot()->GetChildByName("Environment"));
	m_pIntersection1 = static_cast<shoot::MeshEntity*>(this->GetRoot()->GetChildByName("Intersection1"));

	GetMesh()->GetMaterial(0).SetColor(shoot::Color(1.0f, 1.0f, 1.0f, 0.7f));

	// test dotproduct to determine if 2 vectors are facing each other
	//shoot::Vector3D
}

//! called during the update of the entity
/** \param fDeltaTime: time since last Update */
void Player::Update(shoot::f32 fDeltaTime)
{
	super::Update(fDeltaTime);

	bool bRightPressed = shoot::InputManager::Instance()->IsKeyPressed(shoot::InputManager::KT_Right);
	bool bLeftPressed = shoot::InputManager::Instance()->IsKeyPressed(shoot::InputManager::KT_Left);
	
	if(bRightPressed)
	{
		m_vVelocity = m_vSlidingDirection*m_fAcceleration;		
	}
	else if(bLeftPressed)
	{
		m_vVelocity = m_vSlidingDirection*-m_fAcceleration;
	}	
	
	bool bWasInTheAir = IsInTheAir();
	if(bWasInTheAir)
	{
		m_vVelocity.Y += m_fGravity;
	}	

	if(IsInTheAir() || (bRightPressed || bLeftPressed))
	{
		CheckCollision(m_vPosition, m_vVelocity, m_vPosition, m_vVelocity);

		if(bWasInTheAir && !IsInTheAir())
		{
			shoot::Print("*** bWasInTheAir && !IsInTheAir()\n");
		}

		m_vPosition += m_vVelocity;
	}	
}

//! called during the rendering of the entity
void Player::Render()
{
	super::Render();

	// render speed vector
	shoot::Material material;
	shoot::GraphicsDriver::Instance()->SetMaterial(material);
	shoot::GraphicsDriver::Instance()->DrawLine(m_vPosition, m_vPosition+m_vVelocity);
}

// called during the unitialization of the entity
void Player::Uninit()
{
	super::Uninit();
}

//! checks if there is collision
void Player::CheckCollision(const shoot::Vector3D& vPosition, const shoot::Vector3D& vVelocity,
							shoot::Vector3D& vNewPosition, shoot::Vector3D& vNewVelocity)
{
	shoot::Vertex3D* pVertices = m_pEnvironment->GetMesh()->GetVertexBuffer()->GetVertices();
	shoot::u16* pIndices = m_pEnvironment->GetMesh()->GetIndexBuffer(0)->Indices;
	shoot::s32 numIndices = m_pEnvironment->GetMesh()->GetIndexBuffer(0)->NumIndices;
	bool bFound = false;

	for(shoot::s32 i=0; i<numIndices; i += 3)
	{
		shoot::s32 newTriangleIndex = i/3;
		if(newTriangleIndex != m_iCurrentCollidingTriangle) // don't re-process the current colliding triangle
		{
			shoot::Vertex3D v1 = pVertices[pIndices[i + 0]];
			shoot::Vertex3D v2 = pVertices[pIndices[i + 1]];
			shoot::Vertex3D v3 = pVertices[pIndices[i + 2]];

			shoot::Plane plane(v1.Pos, v2.Pos, v3.Pos);

			// only process front facing planes			
			shoot::Plane::E_Classification eObjectClass = plane.ClassifyPoint(vPosition);

			if(eObjectClass == shoot::Plane::C_Front)
			{
				bool bInTheAir = IsInTheAir();
				bool bWall = IsWall(plane);
				shoot::Vector3D vTestVelocity;
				shoot::Vector3D vPointToCheck;

				if(bInTheAir || bWall)
				{
					vTestVelocity = vVelocity;
					vPointToCheck = vPosition - (plane.Normal*m_fRadius);
				}
				else
				{					
					vTestVelocity = shoot::Vector3D(0.0f, -1.0f, 0.0f);
					vPointToCheck = vPosition + shoot::Vector3D(0.0f, -m_fRadius, 0.0f);					
				}

				shoot::Vector3D normalizedVelocity = vTestVelocity;
				normalizedVelocity.Normalize();
				shoot::f32 cosAngle = (normalizedVelocity).DotProduct(plane.Normal);
				if(cosAngle < 0.0f)
				{									
					shoot::Vector3D vDestPoint = vPointToCheck + vTestVelocity;
					shoot::Vector3D vIntersection;
					shoot::Plane::E_Classification eClass;

					bool bIntersection = plane.IntersectWithRay(vPointToCheck, vTestVelocity, &vIntersection, &eClass);

					shoot::Triangle triangle(v1.Pos, v2.Pos, v3.Pos);
					bool bPointInside = bIntersection ? triangle.IsPointInside(vIntersection) : false;

					if(bIntersection && !bPointInside)
					{
						// check if point is on the triangle edges
						//bPointInside =  shoot::Math::IsPointOnLineSegment(vIntersection, triangle.A, triangle.B)
						//			 || shoot::Math::IsPointOnLineSegment(vIntersection, triangle.B, triangle.C)
						//			 || shoot::Math::IsPointOnLineSegment(vIntersection, triangle.C, triangle.A);				
					}

					if(bIntersection 
					&& bPointInside)
					{
						// determine class of dest point
						shoot::Plane::E_Classification eDestClass = plane.ClassifyPoint(vDestPoint);

						// if dest point is not front-facing the plane, we have a collision
						if(eDestClass != shoot::Plane::C_Front)				
						{
							m_pIntersection1->SetPosition(vIntersection);
							m_pIntersection1->GetMesh()->GetMaterial(0).SetColor(shoot::Color::Green);

							shoot::Print("Colliding with triangle '%d' - Normal (%.2f, %.2f, %.2f)\n", i/3, plane.Normal.X, plane.Normal.Y, plane.Normal.Z);				

							if(bWall)
							{
								vNewPosition = vIntersection + plane.Normal*m_fRadius;
								vNewVelocity = shoot::Vector3D(0.0f, 0.0f, 0.0f);														
							}
							else
							{
								vNewPosition = GetNewPosition(vPosition, vIntersection, plane);

								if(bInTheAir)
								{
									vNewVelocity = shoot::Vector3D(0.0f, 0.0f, 0.0f);
								}	

								m_vSlidingDirection = GetSlidingDirection(plane);
								m_iCurrentCollidingTriangle = newTriangleIndex;
							}
							
							shoot::Print("bWall = '%d', vNewVelocity(%.2f, %.2f, %.2f)\n", bWall, vNewVelocity.X, vNewVelocity.Y, vNewVelocity.Z);
							bFound = true;
							break;
						}				
					}			
				}		
			}
		}
	}

	if(!bFound)
	{
		// quickly check if still in contact with current triangle, if not then we are in the air!
		if(m_iCurrentCollidingTriangle >= 0)
		{
			
		}
	}
}

//! returns true if the given triangle is a wall
bool Player::IsWall(const shoot::Plane& plane)
{
	shoot::f32 dotProduct = plane.Normal.DotProduct(shoot::Vector3D(0.0f, 1.0f, 0.0f));
	return shoot::Math::FAbs(dotProduct) < 0.05f;	
}

//! returns true if the player is in the air
bool Player::IsInTheAir() const
{
	return (m_iCurrentCollidingTriangle < 0);
}

//! returns a sliding direction along a plane
shoot::Vector3D Player::GetSlidingDirection(const shoot::Plane& plane)
{
	shoot::Vector3D vDirection = plane.Normal.CrossProduct(shoot::Vector3D(1.0f, 0.0f, 0.0f));

	// Consider the direction as always oriented towards +Z
	if(vDirection.Z < 0.0f)
	{
		vDirection *= -1.0f;
	}
	return vDirection.Normalize();
}

//! returns a new position not colliding with the given plane
shoot::Vector3D Player::GetNewPosition(const shoot::Vector3D& vPosition, const shoot::Vector3D& vIntersection, const shoot::Plane& plane)
{
	shoot::Vector3D vPointToCheck = vPosition - (plane.Normal*m_fRadius);
	shoot::Vector3D vPointToCheckIntersection;
	shoot::Plane::E_Classification eClass;
	bool bIntersection = plane.IntersectWithRay(vPointToCheck, -plane.Normal, &vPointToCheckIntersection, &eClass);

	if(bIntersection) 
	{
		if(eClass == shoot::Plane::C_Back)
		{
			shoot::Vector3D interpenetration = vPointToCheck - vPointToCheckIntersection;
			return vIntersection + shoot::Vector3D(0.0f, m_fRadius+interpenetration.GetLength(), 0.0f);
		}
		else
		{
			return vIntersection + shoot::Vector3D(0.0f, m_fRadius, 0.0f);
		}
	}	
	else
	{
		return vPosition;
	}
}

//! custom Post 3D rendering
void Player::RenderPost3D()
{

}

