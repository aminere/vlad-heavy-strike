/* 

Amine Rehioui
Created: October 29th 2011

*/

using System.Xml;
using System.Reflection;

namespace shoot
{
    //! Object manager class
    public class ObjectManager : Singleton<ObjectManager>
    {
        static string strDefaultMaterialPath = "data/Materials/DefaultMaterial_af3b4b17.xml";

        //! returns an object from an XML
        public Object GetObject(string strPath)
        {
            XmlTextReader xmlReader = new XmlTextReader("Content/" + strPath);
            
            xmlReader.Read();
            string className = xmlReader.Name;
            string assemblyName = Assembly.GetExecutingAssembly().GetName().Name;
            string fullTypeName = assemblyName + "." + className;
            Object _object = Assembly.GetExecutingAssembly().CreateInstance(fullTypeName) as Object;			

            xmlReader.Read();
            PropertyStream stream = new PropertyStream();
            stream.ReadFromXML(xmlReader, null);
            _object.Serialize(stream);

            return _object;
        }

        //! returns the default material object
	    public Object GetDefaultMaterial()
	    {
            return GetObject(strDefaultMaterialPath);
	    }
    }
}

