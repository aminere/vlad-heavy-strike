/* 

Amine Rehioui
Created: October 31st 2011

*/

using System;
using Microsoft.Xna.Framework;

namespace shoot
{
    //! memory stats manager
    public class StatsManager : Singleton<StatsManager>
	{
	    //! constructor
	    public StatsManager()
	    {
		    m_FPS = new Text();
		    m_FPS.SetFontPath("data/Fonts/fixed.xml");
		    m_MemoryUsed = new Text();
		    m_MemoryUsed.SetFontPath("data/Fonts/fixed.xml");
		    m_MemoryUsed.SetPosition(new Vector2(0.0f, 16.0f));
		    m_TextureMemoryUsed = new Text();
		    m_TextureMemoryUsed.SetFontPath("data/Fonts/fixed.xml");
		    m_TextureMemoryUsed.SetPosition(new Vector2(0.0f, 32.0f));
	    }

        //! destructor
	    ~StatsManager()
	    {
		    m_FPS.Uninit();
		    m_MemoryUsed.Uninit();
		    m_TextureMemoryUsed.Uninit();
	    }

		//! update
        public void Update(float fDeltaTime)
        {		    
		    m_FPS.SetText("FPS: " + Engine.Instance().GetFPS());		    
		    m_MemoryUsed.SetText("Mem: ");
		    m_TextureMemoryUsed.SetText("Textures: ");
        }

		//! render
        public void Render()
        {
            m_FPS.Render();
            m_MemoryUsed.Render();
            m_TextureMemoryUsed.Render();
        }

		Text m_FPS;
		Text m_MemoryUsed;
		Text m_TextureMemoryUsed;
	}
}

