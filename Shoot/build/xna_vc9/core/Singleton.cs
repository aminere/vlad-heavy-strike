/* 

Amine Rehioui
Created: September 26th 2010

*/

using System.Diagnostics;

namespace shoot
{
    //! Singleton Class using Generics
    public class Singleton<T> where T: new()
    {
        private static T m_sInstance;

        //! Return the unique instance of class T
        public static T Instance()
        {
            Debug.Assert(m_sInstance != null, "Singleton instance not created yet");
            return m_sInstance;
        }

        //! Creates the instance of class T
        public static T CreateInstance()
        {
            Debug.Assert(m_sInstance == null, "Trying to create Singleton instance twice");
            m_sInstance = new T();
            return m_sInstance;
        }

        //! returns true if the instance was created
        static bool IsCreated() { return m_sInstance != null; }        
    }
}
