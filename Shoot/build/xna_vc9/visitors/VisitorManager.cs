/* 

Amine Rehioui
Created: October 31st 2011

*/

using System.Collections.Generic;
using System.Diagnostics;

namespace shoot
{
    //! Visitor Manager
	public class VisitorManager : Singleton<VisitorManager>
	{
        //! updates the visitors
        public void Update(float fDeltaTime)
        {
            foreach(Visitor v in m_aActiveVisitors)
            {
                if (!v.Update(fDeltaTime))
                {                    
                    // remove visitor
                    v.Active = false;
                    m_aActiveVisitors.Remove(v);
                }
            }
        }

        private List<Visitor> m_aActiveVisitors = new List<Visitor>();        

        //! starts a visitor
        public void StartVisitor(Visitor visitor)
        {
            Visitor found = m_aActiveVisitors.Find(delegate(Visitor v) { return v == visitor; });
            Debug.Assert(found == null, "Calling StartVisitor twice with the same visitor");
            visitor.Active = true;
		    m_aActiveVisitors.Add(visitor);
        }

        //! removes a visitor
        public void RemoveVisitor(Visitor visitor)
        {
            Visitor found = m_aActiveVisitors.Find(delegate(Visitor v) { return v == visitor; });
            Debug.Assert(found != null, "Trying to remove unexisting visitor");
            visitor.Active = false;
            m_aActiveVisitors.Remove(visitor);            
        }
	}
}

