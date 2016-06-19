/* 

Amine Rehioui
Created: September 26th 2010

*/

using Microsoft.Xna.Framework;

namespace shoot
{
    //! Image entity
    /** Ideal for still backgrounds and panels */
    public class Image : Entity2D
    {
        //! Constructor
		public Image()
        {
        }

		//! serializes the entity to/from a PropertyStream
        public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);
        }

		//! called during the initialization of the entity
        public override void Init()
        {
            base.Init();

            // update bounding box according to texture size
            if(MaterialRef.IsValid() && MaterialRef.Get().GetTexture(0) != null)
            {
                m_BoundingBox.Min = new Vector3(0.0f, 0.0f, 0.0f);
                m_BoundingBox.Max = new Vector3(MaterialRef.Get().GetTexture(0).Width, MaterialRef.Get().GetTexture(0).Height, 0.0f);                
            }
        }

		//! called during the update of the entity
		/** \param fDeltaTime: time since last Update */
        public override void Update(float fDeltaTime)
        {
            base.Update(fDeltaTime);
        }
			
		//! called during the rendering of the entity
        public override void Render()
        {
            Engine.Instance().MainEffect.World = GetTransformationMatrix();
            if(MaterialRef.IsValid())
            {
                MaterialRef.Get().Apply();
            }     

            Engine.Instance().MainEffect.CommitChanges();
            Vector2 size = new Vector2(m_BoundingBox.Max.X - m_BoundingBox.Min.X, m_BoundingBox.Max.Y - m_BoundingBox.Min.Y);
            GraphicsManager.Instance().Draw2DQuad(size, m_bHFlip, m_bVFlip);           
        }

		//! registers the entity and its children for rendering
		/** override this to pick an appropriate rendering pass for your entity */
        public override void RegisterForRendering()
        {
            EntityRenderer.Instance().RegisterEntity(this, EntityRenderer.E_RenderPass.RP_2D);

            base.RegisterForRendering();
        }

		// called during the unitialization of the entity
        public override void Uninit()
        {
            base.Uninit();
        }
    }
}
