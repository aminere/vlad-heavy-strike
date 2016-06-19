/* 

Amine Rehioui
Created: September 26th 2010

*/

using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System.Diagnostics;

namespace shoot
{
    //! Material
    public class Material : Object
    {
        const int MaxTextures = 4;

        // properties
		private int m_Mask = 0;
        private Color m_Color = Color.White;
        private List<string> m_strTextures = new List<string>();
        private Texture2D[] m_aTextures = new Texture2D[MaxTextures];

		//! Material flags
		public enum E_Flag
		{
			MF_Lighting = 0x1
		};

		//! constructor
		public Material()
        {
        }

        //! applies this material to upcoming geometry
        public void Apply()
        {
            Vector3 color = new Vector3((float)m_Color.R / 255.0f, (float)m_Color.G / 255.0f, (float)m_Color.B / 255.0f);
            Engine.Instance().MainEffect.DiffuseColor = color;

            Texture2D texture = GetTexture(0);
            if(texture != null)
            {
                Engine.Instance().MainEffect.TextureEnabled = true;
                Engine.Instance().MainEffect.Texture = texture;

                if(GetColor().A < 255
                || texture.Format == SurfaceFormat.Color)
			    {
                    Engine.Instance().MainEffect.Alpha = (float)GetColor().A / 255.0f;
				    Engine.Instance().Device.RenderState.AlphaBlendEnable = true;                    
                    Engine.Instance().Device.RenderState.SourceBlend = Blend.SourceAlpha;
                    Engine.Instance().Device.RenderState.DestinationBlend = Blend.InverseSourceAlpha;                    
			    }
			    else
			    {
				    Engine.Instance().Device.RenderState.AlphaBlendEnable = false;
			    }	
            }
            else
            {
                Engine.Instance().MainEffect.TextureEnabled = false;
            }		    
        }

		//! Reads/Writes struct properties from/to a stream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            bool bLighting = stream.Serialize<bool>("Lighting");
            SetFlag(E_Flag.MF_Lighting, bLighting);            

		    m_Color = stream.Serialize<Color>("Color");
            m_Color.A = (byte)(stream.Serialize<float>("Alpha")*255.0f);
    	
            stream.SerializeArray<string>("Textures", m_strTextures);
            for(int i=0; i<m_strTextures.Count; ++i)
            {
                if(m_strTextures[i].Length > 0)
                {
                    // remove extension
                    string path = m_strTextures[i].Split('.')[0];
                    m_aTextures[i] = Engine.Instance().Content.Load<Texture2D>(path);                
                }
                else
                {
                    m_aTextures[i] = null;
                }
            }		    
        }
				
		//! sets a flag status
		public void SetFlag(E_Flag eFlag, bool bEnabled)
        { 
            if(bEnabled)
            {
                m_Mask |= (int)eFlag;
            }
            else
            {
                m_Mask &= ~(int)eFlag;
            }
        }

		//! returns a flag status
		public bool GetFlag(E_Flag eFlag) { return (m_Mask & (int)eFlag) != 0; }
		
		//! sets the color
		public void SetColor(Color color) { m_Color = color; }

		//! returns the color
		public Color GetColor() { return m_Color; }

		//! sets a texture
		public void SetTexture(Texture2D texture, int index)
        {
            Debug.Assert(index < MaxTextures, "Material.SetTexture: index out of bounds");            
		    m_aTextures[index] = texture;

		    // if array was not initialized, create empty elements in it
		    for(int i=0; i < (index + 1 - m_strTextures.Count); ++i)
		    {
			    m_strTextures.Add("");             
		    }

            m_strTextures[index] = (texture != null ? texture.Name : "");
        }

		//! returns a texture
		public Texture2D GetTexture(int index) { return m_aTextures[index]; }		
    }
}
