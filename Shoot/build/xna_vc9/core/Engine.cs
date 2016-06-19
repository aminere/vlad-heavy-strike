/* 

Amine Rehioui
Created: September 26th 2010

*/

using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Diagnostics;

namespace shoot
{
    //! Shoot Engine base class
    public class Engine
    {       
        private ContentManager m_ContentManager;
        private GraphicsDevice m_GraphicsDevice;
        private BasicEffect m_MainEffect;
        private ContextStack m_ContextStack;
        private static Engine m_sInstance = null;

        // fps calculation
        float m_fps = 0.0f;
        float m_frameTimer = 0.0f;
        uint m_numFrames = 0;

        //! Return the unique instance of the engine
        public static Engine Instance()
        {
            Debug.Assert(m_sInstance != null, "Engine instance not created yet");
            return m_sInstance;
        }

        //! constructor
        public Engine()
        {
            Debug.Assert(m_sInstance == null, "Trying to create Engine instance twice");
            m_sInstance = this;
        }

        //! initializes the engine
        public void Init()
        {
            // create core singletons    		
            EntityRenderer.CreateInstance();
            GraphicsManager.CreateInstance();
            ObjectManager.CreateInstance();
            VisitorManager.CreateInstance();
            StatsManager.CreateInstance();
            m_ContextStack = new ContextStack();
        }

        //! Loads a new context stack after clearing the existing one
		public void LoadContextStack(string path)
        {
            PopAllContexts(true);            

            m_ContextStack = new ContextStack();
            m_ContextStack.LoadFromXML(path, null);
            m_ContextStack.Init();
        }

		//! pushes a context into the stack
		public void PushContext(Context context)
        {
            m_ContextStack.AddChild(context);            
        }

		//! pops the top context
		public void PopContext(bool bImmediate)
        {
            Debug.Assert(GetNumContexts() > 0, "PopContext called on an empty stack");            
            if (bImmediate)
            {
                // TODO: check if context is managed externally
                int index = GetNumContexts() - 1;
                Context context = GetContext(index);
                m_ContextStack.RemoveChild(context);
                context.Uninit();                
            }
            else
            {
                // TODO
            }
        }

		//! removes a context by index
		public void RemoveContext(int index, bool bImmediate)
        {
            Debug.Assert(index < GetNumContexts(), "Context index out of range");            
            if (bImmediate)
            {
                Context context = GetContext(index);
                m_ContextStack.RemoveChild(context);
                context.Uninit();                
            }
            else
            {
                // TODO
            }
        }

		//! pops all the contexts in the stack
		public void PopAllContexts(bool bImmediate)
        {
            if (bImmediate)
            {
                while(GetNumContexts() > 0)
                {
                    PopContext(bImmediate);
                }
                m_ContextStack.Uninit();
            }
            else
            {
                // TODO
            }
        }

		//! returns the number of contexts in the stack
		public int GetNumContexts() { return m_ContextStack.GetNumChildren(); }

		//! returns a context by index
        public Context GetContext(int index) { return m_ContextStack.GetChild(index) as Context; }

		//! returns the context stack
        public ContextStack GetContextStack() { return m_ContextStack; }

		//! called during the update of the engine
		/** \param fDeltaTime: time since last Update 
			\return: false to exit the program */
        public bool Update(float fDeltaTime)
        {
            // calc FPS
            m_frameTimer += fDeltaTime;
            m_numFrames++;
            if (m_frameTimer >= 1.0f)
            {
                m_fps = m_numFrames / m_frameTimer;
                m_numFrames = 0;
                m_frameTimer = 0.0f;
            }

            m_ContextStack.Update(fDeltaTime);
            VisitorManager.Instance().Update(fDeltaTime);
            StatsManager.Instance().Update(fDeltaTime);
            return true;
        }
			
		//! called during the rendering the engine
        public void Render()
        {
            Device.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, default(Color), 1.0f, 0);            

            MainEffect.Begin();
            MainEffect.CurrentTechnique.Passes[0].Begin();
		    
		    m_ContextStack.RegisterForRendering();
		
		    EntityRenderer.Instance().Render();

            MainEffect.CurrentTechnique.Passes[0].End();
            MainEffect.End();            
        }

        //! gets/sets the content manager
        public ContentManager Content
        {
            get
            {
                return m_ContentManager;
            }
            set
            {
                m_ContentManager = value;
            }
        }

        //! gets/sets the graphics device
        public GraphicsDevice Device
        {
            get
            {
                return m_GraphicsDevice;
            }
            set
            {                
                m_GraphicsDevice = value;
                m_MainEffect = new BasicEffect(m_GraphicsDevice, null);
            }
        }

        //! gets the main effect object
        public BasicEffect MainEffect
        {
            get
            {
                return m_MainEffect;
            }
        }

        //! returns the engine fps
		public float GetFPS() { return m_fps; }
    }
}
