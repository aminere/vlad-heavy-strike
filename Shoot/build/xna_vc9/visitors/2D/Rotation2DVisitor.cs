/* 

Amine Rehioui
Created: October 31st 2011

*/

using System.Diagnostics;
using System;

namespace shoot
{
    //! Rotation2DVisitor class
	public class Rotation2DVisitor : AnimationVisitor
	{
		//! constructor
		public Rotation2DVisitor()
        {
        }

        //! destructor
        ~Rotation2DVisitor()
        {
        }

		//! Reads/Writes struct properties from/to a stream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            stream.SerializeStruct("Src", m_Src);
            stream.SerializeStruct("Dest", m_Dest);
            m_fSpeed = stream.Serialize<float>("Speed");
        }

		//! visits a particular entity
        public override void Visit(Entity target)
        {
            m_2DEntity = target as Entity2D;
	        Debug.Assert(m_2DEntity != null, "Rotation2DVisitor target is not of type Entity2D");

	        if(m_2DEntity != null)
	        {
		        m_fInterpolator = 0.0f;                
                ESP<float>.Compute(m_Src, m_Dest, out m_fSrcAngle, out m_fDestAngle, m_2DEntity.GetRotation()+m_Src.Value, m_fSrcAngle+m_Dest.Value);
		        m_fDeltaAngle = Math.Abs(m_fDestAngle-m_fSrcAngle);

		        base.Visit(target);
	        }
        }

        //! updates the visitor
        /** returns false if the visitor is done */
        public override bool Update(float fDeltaTime)
        {
		    Debug.Assert(m_Active, "Trying to update inactive Visitor");            
		    if(m_fInterpolator < 1.0f && !MathUtils.FIsZero(m_fDeltaAngle))
		    {
			    float angle = MathUtils.Lerp(m_fSrcAngle, m_fDestAngle, m_fInterpolator);			
			    m_2DEntity.SetRotation(angle);

			    m_fInterpolator += (m_fSpeed*fDeltaTime)/m_fDeltaAngle;
			    return true;
		    }		
		    else
		    {
			    m_2DEntity.SetRotation(m_fDestAngle);
			    m_PlayCount++;

			    switch(m_PlaybackType)
			    {
			    case PlaybackType.Once:
				    return false;

			    case PlaybackType.Loop:
			    case PlaybackType.Toggle:	
				    if((m_MaxPlayCount < 0) || (m_PlayCount < m_MaxPlayCount))
				    {
                        if (m_PlaybackType == PlaybackType.Toggle) MathUtils.Swap(ref m_fSrcAngle, ref m_fDestAngle);
					    m_fInterpolator = 0.0f;
					    return true;
				    }
				    else
				    {
					    return false;
				    }
    			
			    default:
				    return false;								
			    }			
		    }
        }

        Entity2D m_2DEntity;

        float m_fInterpolator;
        float m_fSrcAngle = 0.0f;
        float m_fDestAngle = 0.0f;
        float m_fDeltaAngle = 0.0f;

        // properties
        ESP<float> m_Src = new ESP<float>();
        ESP<float> m_Dest = new ESP<float>();
        float m_fSpeed = 1.0f;
    }
}

