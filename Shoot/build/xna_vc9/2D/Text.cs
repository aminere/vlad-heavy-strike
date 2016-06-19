/* 

Amine Rehioui
Created: October 30th 2011

*/

using Microsoft.Xna.Framework;
using System.Diagnostics;

namespace shoot
{
    //! Text entity
	public class Text : Entity2D				 
	{
		//! Constructor
		public Text()
        {
        }

		//! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
        {
		    base.Serialize(stream);
            m_strText = stream.Serialize<string>("Text").ToCharArray();
            m_strFontPath = stream.Serialize<string>("FontPath");
        }

		//! called during the initialization of the entity
		public override void Init()
        {
            base.Init();

			Debug.Assert(m_strFontPath.Length > 0, "Creating text with no font");			
			SetFontPath(m_strFontPath);
        }
			
		//! called during the rendering of the entity
		public override void Render()
        {
		    if(m_strText.Length > 0)
		    {
                Engine.Instance().MainEffect.World = GetTransformationMatrix();                
                MaterialRef.Get().Apply();
                Engine.Instance().MainEffect.CommitChanges();

                Vector2 characterPos = Vector2.Zero;
                Vector2 characterSize = m_Font.GetCharacterSize();
                                
                for(int i=0; i<m_strText.Length; ++i)
                {                    
                    char c = m_strText[i];
                    int index = (c - ' ');
                    int row = index / m_Font.GetNumCharactersPerLine();
                    int column = index % m_Font.GetNumCharactersPerLine();

                    Vector3 uvSize = new Vector3(characterSize.X/m_Font.GetTextureSize().X, characterSize.Y/m_Font.GetTextureSize().Y, 0.0f);
                    Vector3 uvStart = new Vector3(column*uvSize.X, row*uvSize.Y, 0.0f);
                    Vector3 uvEnd = uvStart + uvSize;

                    BoundingBox srcRect = new BoundingBox(uvStart, uvEnd);
                    BoundingBox destRect = new BoundingBox(new Vector3(characterPos.X, 0.0f, 0.0f), new Vector3(characterPos.X + characterSize.X, characterSize.Y, 0.0f));
                    GraphicsManager.Instance().Draw2DQuad(srcRect, destRect);

                    characterPos.X += characterSize.X;
                }			
		    }
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
		    m_Font = null;
		    base.Uninit();
        }

		//! Get the local axis-aligned bounding box
		public override BoundingBox GetBoundingBox()
        {
            return new BoundingBox(Vector3.Zero, new Vector3(m_Font.GetCharacterSize().X * m_strText.Length, m_Font.GetCharacterSize().Y, 0.0f));		    
        }

		//! Reloads the font given a font path
		public void SetFontPath(string strPath)
        {
		    m_Font = new Font(strPath);

		    // if no material exists, create it
		    if(!MaterialRef.IsValid())
		    {
			    MaterialRef.Set(ObjectManager.Instance().GetDefaultMaterial() as Material);
		    }

		    MaterialRef.Get().SetTexture(m_Font.GetTexture(), 0);

		    m_strFontPath = strPath;
        }

		//! sets the text
		public void SetText(string strText) { m_strText = strText.ToCharArray(); }

		Font m_Font;

		// properties
        char[] m_strText;
		string m_strFontPath;
	}
}
