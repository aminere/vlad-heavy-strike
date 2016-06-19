/* 

Amine Rehioui
Created: September 19th 2010

*/

using Microsoft.Xna.Framework;

namespace shoot
{
    //! Base 2D entity class
    public class Entity2D : RenderableEntity
    {
        // properties
        private Vector2 m_vPosition = new Vector2(0.0f, 0.0f);
        private float m_fRotation = 0.0f;
        private Vector2 m_vScale = new Vector2(1.0f, 1.0f);
        private Vector2 m_vCenter = new Vector2(0.0f, 0.0f);
        protected BoundingBox m_BoundingBox = new BoundingBox();
        protected bool m_bHFlip = false;
        protected bool m_bVFlip = false;

        //! constructor
        public Entity2D()
        {         
        }

		//! serializes the entity to/from a PropertyStream
        public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_vPosition = stream.Serialize<Vector2>("Position");
            m_fRotation = stream.Serialize<float>("Rotation");  
            m_vScale = stream.Serialize<Vector2>("Scale");
            m_vCenter = stream.Serialize<Vector2>("Center");
            m_BoundingBox = stream.Serialize<BoundingBox>("BBox");
            m_bHFlip = stream.Serialize<bool>("HFlip");
            m_bVFlip = stream.Serialize<bool>("VFlip");
        }

		//! Sets the position relative to the parent's position
		public void SetPosition(Vector2 vPos) { m_vPosition = vPos; }		

		//! Get the position relative to the parent's position
		public Vector2 GetPosition() { return m_vPosition; }

		//! Translates the entity
        public virtual void Translate(Vector2 amount)
        {
            m_vPosition += amount;
        }

		//! returns the rotation
		public float GetRotation() { return m_fRotation; }

		//! returns the scaling vector
		public Vector2 GetScale() { return m_vScale; }

		//! sets the scaling vector
		public void SetScale(Vector2 vScale) { m_vScale = vScale; }

		//! sets the scale uniformally
		public void SetScale(float fScale) { m_vScale = new Vector2(fScale, fScale); }

		//! sets the rotation
		public void SetRotation(float fRotation) { m_fRotation = fRotation; }

		//! returns the center of local transformation 
		public Vector2 GetCenter() { return m_vCenter; }

		//! sets the center of local transformation
		public void SetCenter(Vector2 vCenter) { m_vCenter = vCenter; }

		//! returns the affine transformation matrix
		/** TODO: avoid recalculating when nothing changed */
        public Matrix GetTransformationMatrix()
        {
            Matrix matrix = Matrix.Identity;     
            matrix.Translation = new Vector3(GetCenter().X, GetCenter().Y, 0.0f)*-1.0f;
            matrix *= Matrix.CreateScale(new Vector3(GetScale().X, GetScale().Y, 1.0f));
            matrix *= Matrix.CreateRotationZ(-MathHelper.ToRadians(GetRotation()));
            matrix *= Matrix.CreateTranslation(new Vector3(GetPosition().X, GetPosition().Y, 0.0f));

            if (GetParent() != null && GetParent() is Entity2D)
            {
                matrix *= (GetParent() as Entity2D).GetTransformationMatrix();
            }

            return matrix;
        }

		//! Sets the local axis-aligned bounding box
		public void SetBoundingBox(BoundingBox bbox) { m_BoundingBox = bbox; }

		//! Get the local axis-aligned bounding box
		public virtual BoundingBox GetBoundingBox() { return m_BoundingBox; }

		//! returns the horizontal flipping status
		public bool IsHFlipped() { return m_bHFlip; }

		//! set horizontal flipping
		public void SetHFlip(bool bSet) { m_bHFlip = bSet; }

		//! returns the horizontal flipping status
		public bool IsVFlipped() { return m_bVFlip; }

		//! set horizontal flipping
		public void SetVFlip(bool bSet) { m_bVFlip = bSet; }
    }
}
