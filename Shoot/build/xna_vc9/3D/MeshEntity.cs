/* 

Amine Rehioui
Created: October 12th 2010

*/

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! 3D Mesh entity
    public class MeshEntity : Entity3D
    {
        private Model m_Model;

        // properties
		private string m_strPath;

        //! constructor
		public MeshEntity()
        {
        }

		//! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_strPath = stream.Serialize<string>("Path");            
        }

		//! called during the initialization of the entity
		public override void Init()
        {
            base.Init();

            // remove reference to data folder and remove extension
            string path = m_strPath.Split('.')[0];
            m_Model = Engine.Instance().Content.Load<Model>(path);            

            // if no material is set create a default one to avoid affecting other entities
            if (!MaterialRef.IsValid())
            {
                Material material = new Material();
                MaterialRef.Set(material);
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
            MaterialRef.Get().Apply();                     

            Engine.Instance().MainEffect.CommitChanges();

            foreach (ModelMesh mesh in m_Model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    //effect.EnableDefaultLighting();
                    effect.World = GetTransformationMatrix();
                    effect.View = Engine.Instance().MainEffect.View;
                    effect.Projection = Engine.Instance().MainEffect.Projection;
                }

                mesh.Draw();
            }            
        }

		//! registers the entity and its children for rendering
		/** override this to pick an appropriate rendering pass for your entity */
		public override void RegisterForRendering()
        {
            EntityRenderer.Instance().RegisterEntity(this, EntityRenderer.E_RenderPass.RP_3D);

            base.RegisterForRendering();
        }

		// called during the unitialization of the entity
		public override void Uninit()
        {
            base.Uninit();
        }		
    }
}
