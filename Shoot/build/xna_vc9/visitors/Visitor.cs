/* 

Amine Rehioui
Created: October 31st 2011

*/

using System.Diagnostics;

namespace shoot
{
	//! Base Visitor class
	public class Visitor : Object
	{		
		//! constructor
		public Visitor()
        {
        }

        //! destructor
        ~Visitor()
        {
            Leave();
        }

		//! Reads/Writes struct properties from/to a stream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);
            m_AutoStart = stream.Serialize<bool>("AutoStart");
        }

		//! visits a particular entity
		public virtual void Visit(Entity target)
        {
		    Debug.Assert(target != null, "Cannot Visit a NULL entity");
		    m_Target = target;
		    VisitorManager.Instance().StartVisitor(this);
        }

		//! Leaves the visited entity
		public virtual void Leave()
        {
		    if(m_Target != null && m_Active)
		    {
			    m_Target = null;
			    VisitorManager.Instance().RemoveVisitor(this);
		    }
        }

		//! updates the visitor
		/** returns false if the visitor is done */
		public virtual bool Update(float fDeltaTime) { return false; }

        //! Access AutoStart status
		public bool AutoStart
        {
            get { return m_AutoStart; }
        }

        //! Access Active status
        public bool Active
        {
            get { return m_Active; }
            set { m_Active = value; }
        }		
        
		protected Entity m_Target;
		protected bool m_Active = false;

		// properties
		protected bool m_AutoStart = true;
	}
}

