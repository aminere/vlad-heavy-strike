/* 

Amine Rehioui
Created: September 19th 2010

*/

using Microsoft.Xna.Framework;

namespace shoot
{
    //! Base 3D entity class
    public class Entity3D : RenderableEntity
    {
        // properties
        private Vector3 m_vPosition = new Vector3(0.0f, 0.0f, 0.0f);
        private Vector3 m_vRotation = new Vector3(0.0f, 0.0f, 0.0f);
        private Vector3 m_vScale = new Vector3(1.0f, 1.0f, 1.0f);
        private Vector3 m_vCenter = new Vector3(0.0f, 0.0f, 0.0f);
        protected BoundingBox m_BoundingBox = new BoundingBox();

        //! constructor
        public Entity3D()
        {
        }

        //! serializes the entity to/from a PropertyStream
        public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_vPosition = stream.Serialize<Vector3>("Position");
            m_vRotation = stream.Serialize<Vector3>("Rotation");
            m_vScale = stream.Serialize<Vector3>("Scale");
            m_vCenter = stream.Serialize<Vector3>("Center");
            m_BoundingBox = stream.Serialize<BoundingBox>("BBox");
        }

        //! Sets the position relative to the parent's position
        public void SetPosition(Vector3 vPos) { m_vPosition = vPos; }

        //! Get the position relative to the parent's position
        public Vector3 GetPosition() { return m_vPosition; }

        //! Translates the entity
        public virtual void Translate(Vector3 amount)
        {
            m_vPosition += amount;
        }

        //! returns the rotation
        public Vector3 GetRotation() { return m_vRotation; }

        //! returns the scaling vector
        public Vector3 GetScale() { return m_vScale; }

        //! sets the scaling vector
        public void SetScale(Vector3 vScale) { m_vScale = vScale; }

        //! sets the scale uniformally
        public void SetScale(float fScale) { m_vScale = new Vector3(fScale, fScale, fScale); }

        //! sets the rotation
        public void SetRotation(Vector3 vRotation) { m_vRotation = vRotation; }

        //! returns the center of local transformation 
        public Vector3 GetCenter() { return m_vCenter; }

        //! sets the center of local transformation
        public void SetCenter(Vector3 vCenter) { m_vCenter = vCenter; }

        //! returns the affine transformation matrix
        /** TODO: avoid recalculating when nothing changed */
        public Matrix GetTransformationMatrix()
        {
            Matrix matrix = Matrix.Identity;
            matrix.Translation = GetCenter() * -1.0f;
            matrix *= Matrix.CreateScale(GetScale());
            matrix *= Matrix.CreateRotationX(-MathHelper.ToRadians(GetRotation().X));
            matrix *= Matrix.CreateRotationY(-MathHelper.ToRadians(GetRotation().Y));
            matrix *= Matrix.CreateRotationZ(-MathHelper.ToRadians(GetRotation().Z));
            matrix *= Matrix.CreateTranslation(GetPosition());

            if (GetParent() != null && GetParent() is Entity3D)
            {
                matrix *= (GetParent() as Entity3D).GetTransformationMatrix();
            }

            return matrix;
        }

        //! Sets the local axis-aligned bounding box
        public void SetBoundingBox(BoundingBox bbox) { m_BoundingBox = bbox; }

        //! Get the local axis-aligned bounding box
        public virtual BoundingBox GetBoundingBox() { return m_BoundingBox; }
    }
}
