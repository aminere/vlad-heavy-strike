/* 

Amine Rehioui
Created: October 29th 2011

*/

namespace shoot
{
    //! Base Object class
    public class Object
    {
        // properties
        private uint m_ID = 0;
        private string m_Name = "";

        //! Constructor
		public Object()
        {
        }

		//! serializes the entity to/from a PropertyStream
        public virtual void Serialize(PropertyStream stream)
        {
            m_ID = stream.Serialize<uint>("ID");
            m_Name = stream.Serialize<string>("Name");
        }

        //! returns the ID of this entity
        public uint GetID() { return m_ID; }

        //! sets the ID of this entity
        public void SetID(uint ID) { m_ID = ID; }

        //! returns the name of this entity
        public string GetName() { return m_Name; }

        //! sets the name of this entity
        public void SetName(string name) { m_Name = name; }
    }
}

