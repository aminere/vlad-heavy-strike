/* 

Amine Rehioui
Created: September 19th 2010

*/

using System.Collections.Generic;
using System.Xml;
using System.Reflection;
using System.Diagnostics;

namespace shoot
{
    //! Class to encapsulate properties and communicate with XML
    public class PropertyStream
    {       
        private List<IProperty> m_aProperties;

        //! constructor
        public PropertyStream()
        {
            m_aProperties = new List<IProperty>();
        }        

        //! returns the property with the specified name
        IProperty GetProperty(string name)
        {
            foreach(IProperty property in m_aProperties)
            {                
                if (property.GetName() == name)
                {
                    return property;
                }
            }            
            return null;
        }

        //! serializes a property
        public T Serialize<T>(string name)
        {
            IProperty property = GetProperty(name);
            if (property != null)
            {
                T value = (T)property.GetValue();
                return value;
            }
            return default(T);
        }

        //! serializes a struct property
        public void SerializeStruct(string name, ISerializableStruct userStruct)
        {
            IProperty property = GetProperty(name);
            if (property != null)
            {
                StructProperty structProperty = property as StructProperty;
                userStruct.Serialize(structProperty.GetStream());
            }
        }

        //! serializes an array property
        public void SerializeArray<T>(string name, List<T> userArray)
        {
            IProperty property = GetProperty(name);
            if (property != null)
            {
                ArrayProperty arrayProperty = property as ArrayProperty;
                userArray.Clear();

                foreach (IProperty subProperty in arrayProperty.GetProperties())
                {
                    Debug.Assert((subProperty is ArrayProperty) == false, "Arrays of arrays not supported. Encapsulate your inner array into a struct to get around this");
                    Debug.Assert((subProperty is StructProperty) == false, "Cannot serialize reference types, use SerializeArrayRef instead");
                    T element = (T)subProperty.GetValue();
                    userArray.Add(element);
                }
            }
        }

        //! serializes an array property - reference types
        public void SerializeArrayRef<T>(string name, List<T> userArray) where T : new()
        {
            IProperty property = GetProperty(name);
            if (property != null)
            {
                ArrayProperty arrayProperty = property as ArrayProperty;
                userArray.Clear();

                foreach (IProperty subProperty in arrayProperty.GetProperties())
                {
                    Debug.Assert((subProperty is ArrayProperty) == false, "Arrays of arrays not supported. Encapsulate your inner array into a struct to get around this");
                    T element = new T();

                    if (subProperty is StructProperty)
                    {
                        StructProperty structProperty = subProperty as StructProperty;
                        ISerializableStruct userStruct = element as ISerializableStruct;
                        userStruct.Serialize(structProperty.GetStream());
                    }
                    else
                    {
                        element = (T)subProperty.GetValue();
                    }

                    userArray.Add(element);
                }
            }
        }

        //! serializes a reference property
        public void SerializeReference<T>(string name, Reference<T> userContainer) where T : class, new()
        {
            IProperty property = GetProperty(name);
            if (property != null)
            {
                ReferenceProperty refProperty = property as ReferenceProperty;

                bool unique = (refProperty.TemplatePath != null && refProperty.TemplatePath.Length > 0);
		        if(refProperty.GetStream().m_aProperties.Count > 0 || unique)
		        {
			        T _object = userContainer.Get();
                    string className = (_object != null) ? typeof(T).ToString() : null;
			        bool createNewObject = (_object == null)
								          || (!unique && (className != refProperty.ClassName));
			        if(createNewObject)
			        {        				
				        if(unique)
				        {
					        string templatePath = refProperty.TemplatePath;
					        _object = ObjectManager.Instance().GetObject(templatePath) as T;
				        }
				        else
				        {
                            string assemblyName = Assembly.GetExecutingAssembly().GetName().Name;
                            string fullTypeName = assemblyName + "." + refProperty.ClassName;
                            _object = Assembly.GetExecutingAssembly().CreateInstance(fullTypeName) as T;
				        }

                        userContainer.Set(_object);				        
			        }

                    if (refProperty.GetStream().m_aProperties.Count > 0)
			        {
                        Object _objectCast = _object as Object;
                        _objectCast.Serialize(refProperty.GetStream());
			        }
		        }
            }
        }

        //! fills this stream from an xml element
        public void ReadFromXML(XmlTextReader xmlReader, ArrayProperty parentArray)
        {
            while(xmlReader.Read())
            {
                if(xmlReader.NodeType == XmlNodeType.EndElement)
                {
                    break;
                }
                else if (xmlReader.NodeType == XmlNodeType.Element)
                {
                    // get the property type name
                    string propertyTypeName = xmlReader.Name;                  
                    IProperty property = IProperty.CreateFromTypeName(propertyTypeName);

                    // get the property name
                    string strPropertyName = xmlReader.GetAttribute("Name");
                    if (strPropertyName != null)
                    {
                        property.SetName(strPropertyName);
                    }

                    if (property is ReferenceProperty)
                    {
                        ReferenceProperty refProperty = property as ReferenceProperty;
                        // read the class names
                        string className = xmlReader.GetAttribute("ClassName");
                        string templatePath = xmlReader.GetAttribute("TemplatePath");
                        Debug.Assert(className.Length > 0 || templatePath.Length > 0, "property missing both ClassName and TemplatePath");
                        if (className.Length > 0)
                        {
                            refProperty.ClassName = className;
                            if (!xmlReader.IsEmptyElement)
                            {
                                refProperty.GetStream().ReadFromXML(xmlReader, null);
                            }
                        }
                        else
                        {
                            refProperty.TemplatePath = templatePath;
                        }
                    }
                    else if (property is StructProperty)
                    {
                        StructProperty structProperty = property as StructProperty;
                        structProperty.GetStream().ReadFromXML(xmlReader, null);
                    }                    
                    else if(property is ArrayProperty)
                    {
                        if (!xmlReader.IsEmptyElement)
                        {
                            ReadFromXML(xmlReader, property as ArrayProperty);
                        }                        
                    }                     
                    else
                    {
                        // get the property value
                        string strPropertyValue = xmlReader.GetAttribute("Value");
                        Debug.Assert(strPropertyValue != null, strPropertyName + " property missing Value attribute");
                        property.SetValue(strPropertyValue);
                    }

                    if (parentArray != null)
                    {
                        parentArray.GetProperties().Add(property);
                    }                    
                    else
                    {
                        m_aProperties.Add(property);
                    }
                } 
			} // end while
        }               
    }
}

