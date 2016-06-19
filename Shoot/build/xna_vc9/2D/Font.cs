/* 

Amine Rehioui
Created: October 30th 2011

*/

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Xml;
using System.Diagnostics;

namespace shoot
{
    //! Bitmap font class
	public class Font
	{
		//! constructor
		public Font(string strPath)
        {
            XmlTextReader xmlReader = new XmlTextReader("Content/" + strPath);
            xmlReader.Read();
            Debug.Assert(xmlReader.Name == "Font", "Invalid font file");
            xmlReader.Read();
            PropertyStream stream = new PropertyStream();
            stream.ReadFromXML(xmlReader, null);
            Serialize(stream);
            m_Texture = Engine.Instance().Content.Load<Texture2D>(m_strPath.Split('.')[0]);
        }

		//! destructor
		~Font()
        {
        }

		//! serializes the font from a stream
        public void Serialize(PropertyStream stream)
        {
            m_strPath = stream.Serialize<string>("Path");
            m_vCharacterSize = stream.Serialize<Vector2>("CharacterSize");
            m_NumCharactersPerLine = stream.Serialize<int>("CharactersPerLine");
        }

		//! returns the texture
        public Texture2D GetTexture() { return m_Texture; }

		//! returns the size of the texture
        public Vector2 GetTextureSize()
        {
            return new Vector2(m_Texture.Width, m_Texture.Height);            
        }

		//! returns the character size
        public Vector2 GetCharacterSize() { return m_vCharacterSize; }

		//! returns the number of characters per line
        public int GetNumCharactersPerLine() { return m_NumCharactersPerLine; }

		Texture2D m_Texture;

		// properties
		string m_strPath;
		Vector2 m_vCharacterSize;
		int m_NumCharactersPerLine;
	}
}
