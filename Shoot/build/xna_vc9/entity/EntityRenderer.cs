/* 

Amine Rehioui
Created: September 26th 2010

*/

using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! EntityRenderer performs rendering passes on both 2D / 3D entities
    public class EntityRenderer : Singleton<EntityRenderer>
    {
        //! rendering pass types
		public enum E_RenderPass
		{			
			RP_2D,
			RP_3D,
			RP_Transparent2D,	
			RP_Transparent3D,
			RP_SkyBox,
			RP_None,
			RP_Count
		}

		//! registers the entity for a particular rendering pass
		public void RegisterEntity(Entity entity, E_RenderPass ePass)
        {
            switch (ePass)
            {
                case E_RenderPass.RP_2D:
                    m_2DEntities.Add(entity);
                    break;

                case E_RenderPass.RP_3D:
                    m_3DEntities.Add(entity);
                    break;

                case E_RenderPass.RP_Transparent2D:
                    m_Transparent2DEntities.Add(entity);
                    break;

                case E_RenderPass.RP_Transparent3D:
                    m_Transparent3DEntities.Add(entity);
                    break;

                case E_RenderPass.RP_SkyBox:
                    m_SkyBoxEntities.Add(entity);
                    break;

                default:
                    Debug.Assert(false, "Invalid render pass");
                    break;
            }
        }

		//! renders the entities
		public void Render()
        {
            Viewport v = Engine.Instance().Device.Viewport;		    

            // render skyboxes
            Engine.Instance().Device.RenderState.DepthBufferWriteEnable = false;
            Engine.Instance().MainEffect.Projection = Get3DCamera().GetProjectionMatrix(v.Width, v.Height);
            Engine.Instance().MainEffect.World = Matrix.Identity;		    
    				    
            foreach (Entity entity in m_SkyBoxEntities)
            {
               entity.Render();
            }

		    // set up 3D view
            Engine.Instance().Device.RenderState.DepthBufferEnable = true;
            Engine.Instance().Device.RenderState.DepthBufferFunction = Microsoft.Xna.Framework.Graphics.CompareFunction.Less;
            Engine.Instance().Device.RenderState.DepthBufferWriteEnable = true;
            Engine.Instance().MainEffect.View = Get3DCamera().GetViewMatrix();
		    
		    // render 3D entities
            foreach (Entity entity in m_3DEntities)
		    {
			    entity.Render();
		    }

		    // render transparent 3D entities		   
            foreach (Entity entity in m_Transparent3DEntities)
		    {
			    entity.Render();
		    }

		    // post 3D rendering
		    if (m_Post3DRenderers.Count > 0)
		    {			    
                foreach(Post3DRenderer renderer in m_Post3DRenderers)
			    {
				    renderer.RenderPost3D();
			    }
		    }

		    // set up 2D view
            Engine.Instance().Device.Clear(ClearOptions.DepthBuffer, default(Color), 1.0f, 0);		    

		    // change depth func to make last rendered objects pass if their depth is the same as previously rendered objects
            Engine.Instance().Device.RenderState.DepthBufferFunction = Microsoft.Xna.Framework.Graphics.CompareFunction.LessEqual;
            Get2DCamera().Render();

		    // render 2D entities		    
            foreach (Entity entity in m_2DEntities)
		    {
                entity.Render();
		    }

		    // render transparent 2D entities
            foreach (Entity entity in m_Transparent2DEntities)		   
		    {
                entity.Render();
		    }

		    // post 2D rendering
            if (m_Post2DRenderers.Count > 0)
		    {
                foreach (Post2DRenderer renderer in m_Post2DRenderers)			    
			    {
                    renderer.RenderPost2D();
			    }
		    }

            StatsManager.Instance().Render();

		    // clears the rendering lists
		    m_SkyBoxEntities.Clear();
            m_2DEntities.Clear();
            m_3DEntities.Clear();
            m_Transparent2DEntities.Clear();
            m_Transparent3DEntities.Clear();
        }

		//! change the current 3D camera
		/** \param pCamera: taget camera. If NULL, a default camera will be used */
		public void Set3DCamera(Camera3D camera) { m_3DCamera = camera; }

		//! returns the current 3D camera
		Camera3D Get3DCamera() { return m_3DCamera != null ? m_3DCamera : m_Default3DCamera; }

		//! changes the current 2D camera
		/** \param pCamera: taget camera. If NULL, a default camera will be used */
		void Set2DCamera(Camera2D camera) { m_2DCamera = camera; }

		//! returns the current 2D camera
		Camera2D Get2DCamera() { return m_2DCamera != null ? m_2DCamera : m_Default2DCamera; }

		//! registers a post renderer
		/** polymorphism is not used at its best here to allow for a class to be both Post 2D & 3D renderer */
        void AddPostRenderer(PostRenderer postRenderer, PostRenderer.E_Type eType)
        {
		    switch(eType)
		    {
             case PostRenderer.E_Type.T_3D:
                Debug.Assert(postRenderer is Post3DRenderer, "PostRenderer instance is of invalid type");
                m_Post3DRenderers.Add(postRenderer as Post3DRenderer);
			    break;

             case PostRenderer.E_Type.T_2D:
                Debug.Assert(postRenderer is Post2DRenderer, "PostRenderer instance is of invalid type");
                m_Post2DRenderers.Add(postRenderer as Post2DRenderer);
                break;
    			
		    default:
                Debug.Assert(false, "Invalid PostRenderer type");			   
                break;
		    }
        }

	    // private members
        private List<Entity> m_2DEntities = new List<Entity>();
        private List<Entity> m_3DEntities = new List<Entity>();
        private List<Entity> m_Transparent2DEntities = new List<Entity>();
        private List<Entity> m_Transparent3DEntities = new List<Entity>();
        private List<Entity> m_SkyBoxEntities = new List<Entity>();
		
		private List<Post3DRenderer> m_Post3DRenderers = new List<Post3DRenderer>();
        private List<Post2DRenderer> m_Post2DRenderers = new List<Post2DRenderer>();
	
		Camera3D m_3DCamera;
		Camera3D m_Default3DCamera = new Camera3D();

		Camera2D m_2DCamera;
		Camera2D m_Default2DCamera = new Camera2D();		
    }
}
