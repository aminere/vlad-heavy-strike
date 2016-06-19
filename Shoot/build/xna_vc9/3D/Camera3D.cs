/* 

Amine Rehioui
Created: September 26th 2010

*/

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Diagnostics;

namespace shoot
{
    //! 3D Camera entity
    public class Camera3D : Entity3D
    {
        //! camera types
        public enum E_Type
        {
            T_Perspective,
            T_Ortho
        };

        // properties
        float m_FOV = 45.0f;
        float m_ZNear = 0.1f;
        float m_ZFar = 1000.0f;
        E_Type m_eType = E_Type.T_Perspective;
        Vector3 m_vUpVector = new Vector3(0.0f, 1.0f, 0.0f);

        //! constructor
        public Camera3D()
        {
            SetPosition(new Vector3(0.0f, 0.0f, 10.0f));
        }

        //! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_FOV = stream.Serialize<float>("FOV");
            m_ZNear = stream.Serialize<float>("ZNear");
            m_ZFar = stream.Serialize<float>("ZFar");
            m_eType = (E_Type)stream.Serialize<int>("Projection");
            m_vUpVector = stream.Serialize<Vector3>("UpVector");
        }

        //! called during the rendering of the entity		
        public override void Render()
        {
            Viewport v = Engine.Instance().Device.Viewport;
            Engine.Instance().Device.RenderState.DepthBufferEnable = true;
            Engine.Instance().MainEffect.Projection = GetProjectionMatrix(v.Width, v.Height);
            Engine.Instance().MainEffect.View = GetViewMatrix();
        }

        //! returns the projection matrix
        /** TODO: only compute if necessary */
        public Matrix GetProjectionMatrix(int screenWidth, int screenHeight)
        {
            Viewport v = Engine.Instance().Device.Viewport;
            switch (m_eType)
            {
            case E_Type.T_Perspective:
                return Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(m_FOV), (float)v.Width / (float)v.Height, m_ZNear, m_ZFar);
               
            case E_Type.T_Ortho:                
                return Matrix.CreateOrthographic(v.Width, v.Height, m_ZNear, m_ZFar);

            default:
                Debug.Assert(false, "Invalid camera type");               
                return default(Matrix);               
            }
        }

        //! returns the view matrix
        /** TODO: only compute if necessary */
        public Matrix GetViewMatrix()
        {
            return Matrix.CreateLookAt(GetPosition(), GetCenter(), GetUpVector());
        }

        //! returns the Up vector
        public Vector3 GetUpVector() { return m_vUpVector; }
    }
}
