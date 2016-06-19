/* 

Amine Rehioui
Created: September 27th 2010

*/

namespace shoot
{
    //! A context is the logical representation of a game area
    /** Contexts allow smooth transitions between game areas and can be superposed by the engine 
        into a stack where multiple contexts are managed at the same time */
    public class Context : Entity
    {
        private Entity m_ChildEntity;

        // properties
        string m_ChildEntityPath;

        //! Constructor
		public Context()
        {
        }

		//! Constructor
		Context(string path)
        {
            LoadFromXML(path, null);
        }

		//! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);
            
		    m_ChildEntityPath = stream.Serialize<string>("ChildEntityPath");

            if (m_ChildEntityPath.Length > 0)
            {
                m_ChildEntity = new Entity();
                m_ChildEntity.LoadFromXML(m_ChildEntityPath, null);
                m_ChildEntity.Init();
                AddChild(m_ChildEntity);
            }		
        }
    }
}

