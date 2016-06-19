/* 

Amine Rehioui
Created: September 19th 2010

*/

using System.Collections.Generic;
using System.Xml;
using System.Reflection;
using System.Diagnostics;

using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! Base class for all game objects
    public class Entity : Object
    {
        private List<Entity> m_aChildren = new List<Entity>();
        private Entity m_Parent = null;
        private bool m_bInitialized = false;

        // properties
        private bool m_bEnabled = true;
        private bool m_bVisible = true;
        Reference<Visitor> m_VisitorRef = new Reference<Visitor>();

        //! constructor
        public Entity()
        {
           
        }

		//! serializes the entity to/from a PropertyStream
        public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_bEnabled = stream.Serialize<bool>("Enabled");
            m_bVisible = stream.Serialize<bool>("Visible");

            stream.SerializeReference("Visitor", m_VisitorRef);
        }

		//! called during the initialization of the entity
        public virtual void Init()
        {
            foreach (Entity child in m_aChildren)
            {
                child.Init();
            }

            if (m_VisitorRef.IsValid() && m_VisitorRef.Get().AutoStart)
            {
                m_VisitorRef.Get().Visit(this);
            }

            m_bInitialized = true;
        }

		//! called during the update of the entity
		/** \param fDeltaTime: time since last Update */
        public virtual void Update(float fDeltaTime)
        {
            foreach (Entity child in m_aChildren)
            {                
                child.Update(fDeltaTime);
            }            
        }
			
		//! called during the rendering of the entity		
		public virtual void Render()
        {            
        }

		//! registers the entity and its children for rendering
		/** override this to pick an appropriate rendering pass for your entity */
        public virtual void RegisterForRendering()
        {
            foreach (Entity child in m_aChildren)
            {
                child.RegisterForRendering();
            }
        }

		//! called during the unitialization of the entity
        public virtual void Uninit()
        {
            foreach (Entity child in m_aChildren)
            {
                child.Uninit();
            }

            m_aChildren.Clear();

            if (m_VisitorRef.IsValid() && m_VisitorRef.Get().Active)
            {
                m_VisitorRef.Get().Leave();
            }

            m_bInitialized = false;
        }

        //! returns the number of children 
        public int GetNumChildren() { return m_aChildren.Count; }

		//! returns a pointer to a child
		/** \param index: index of the wanted child */
        public Entity GetChild(int index) { return m_aChildren[index] as Entity; }
		
		//! returns a child using its ID
		/** \param ID: ID of the wanted child */
        public Entity GetChildByID(int ID)
        {
            foreach (Entity child in m_aChildren)
            {                
                if (child.GetID() == ID)
                {
                    return child;
                }
            }

            foreach (Entity child in m_aChildren)
            {
                Entity grandChild = child.GetChildByID(ID);
                if (grandChild != null)
                {
                    return grandChild;
                }
            }
            return null;
        }

		//! returns a child using its name
		/** \param strName: name of the wanted child */
        public Entity GetChildByName(string name)
        {
            foreach (Entity child in m_aChildren)
            {                
                if (child.GetName() == name)
                {
                    return child;
                }
            }

            foreach (Entity child in m_aChildren)
            {
                Entity grandChild = child.GetChildByName(name);
                if (grandChild != null)
                {
                    return grandChild;
                }
            }
            return null;
        }

        //! adds a child to the entity
        public virtual void AddChild(Entity child)
        {
            if(child.GetParent() != null)
		    {
			    child.GetParent().RemoveChild(child);
		    }

		    m_aChildren.Add(child);
		    child.m_Parent = this;
        }

		//! inserts a child into a specific index
        public virtual void InsertChild(Entity child, int index)
        {
            if (child.GetParent() != null)
            {
                child.GetParent().RemoveChild(child);
            }

            m_aChildren.Insert(index, child);
            child.m_Parent = this;
        }

		//! removes a child from this entity
        public virtual void RemoveChild(Entity child)
        {
            m_aChildren.Remove(child);
            child.m_Parent = null;
        }
			
		//! returns the parent of this entity
        public Entity GetParent() { return m_Parent; }

		//! returns the root entity
        public Entity GetRoot()
        {
            return (GetParent() == null) ? this : GetParent().GetRoot();            
        }

		//! returns if this entity is enabled or not.
		/** Disabled entities are not updated */
        public bool IsEnabled() { return m_bEnabled; }

		//! enables / disables this entity
        public void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }

		//! returns the visibility status of this entity
        public bool IsVisible() { return m_bVisible; }

		//! sets the visibility status of this entity
        public void SetVisible(bool bVisible) { m_bVisible = bVisible; }

        //! returns the initialization status
        public bool IsInitialized() { return m_bInitialized; }        

        //! Loads the entity and its hierarchy from an XML
        public void LoadFromXML(string strPath, Entity parent)
        {
            XmlTextReader xmlReader = new XmlTextReader("Content/" + strPath);

            while(xmlReader.Read())
            {
                if (xmlReader.NodeType == XmlNodeType.EndElement)
                {
                    break;
                }
                else if (xmlReader.NodeType == XmlNodeType.Element)
                {
                    ReadEntity(xmlReader, parent);
                }
            }

            xmlReader.Close();
        }

        //! recursive read from XML
        void ReadEntity(XmlTextReader xmlReader, Entity parent)
        {
            Entity entity = null;
            string entityTypeName = xmlReader.Name;

            if (parent != null)
            {
                // create entity
                string assemblyName = Assembly.GetExecutingAssembly().GetName().Name;
                string fullTypeName = assemblyName + "." + entityTypeName;
                entity = Assembly.GetExecutingAssembly().CreateInstance(fullTypeName) as Entity;
            }
            else
            {
                string thisTypeName = GetType().Name;
                Debug.Assert(thisTypeName == entityTypeName, "Trying to load an entity of type " + entityTypeName + " into an " + thisTypeName);
                entity = this;
            }

            // read children
            while(xmlReader.Read())
            {
                if(xmlReader.NodeType == XmlNodeType.EndElement)
                {
                    break;
                }
                else if(xmlReader.NodeType == XmlNodeType.Element)
                {
                    if (xmlReader.Name == "Properties")
                    {
                        // read properties                    
                        PropertyStream stream = new PropertyStream();
                        stream.ReadFromXML(xmlReader, null);
                        entity.Serialize(stream);
                    }
                    else
                    {
                        ReadEntity(xmlReader, entity);
                    }
                }              
            } // end while

            if(parent != null)
            {
                // Add entity after it has been serialized to ensure proper event handling inside of AddChild()
                parent.AddChild(entity);
            }
        }
    }
}
