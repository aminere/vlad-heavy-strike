/* 

Amine Rehioui
Created: September 19th 2010

*/

using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! Property interface
    /** A property is used to expose a variable to XML and to visual editors */
    public abstract class IProperty
    {
        private string m_Name;

		//! sets the value of this property from a string
		public abstract void SetValue(string strValue);

        //! returns the value stored in this property
		public abstract object GetValue();		

        //! sets the name of this property
		public void SetName(string name) { m_Name = name; }

		//! returns the name of this property
        public string GetName() { return m_Name; }

        //! create a property from a type name
        public static IProperty CreateFromTypeName(string name)
        {            
            if (name == "int") return new IntProperty(0);
            else if (name == "uint") return new UIntProperty(0);
            else if (name == "float") return new FloatProperty();
            else if (name == "string") return new StringProperty();            
            else if (name == "bool") return new BoolProperty();
            else if (name == "vec2D") return new Vec2DProperty();
            else if (name == "vec3D") return new Vec3DProperty();
            else if (name == "struct") return new StructProperty();
            else if (name == "array") return new ArrayProperty();
            else if (name == "color") return new ColorProperty();
            else if (name == "file") return new StringProperty();
            else if (name == "point") return new Vec2DProperty();
            else if (name == "aabb2D") return new BoundingBoxProperty();
            else if (name == "aabb3D") return new BoundingBoxProperty();
            else if (name == "size") return new Vec2DProperty();
            else if (name == "enum") return new IntProperty(0);
            else if (name == "link") return new UIntProperty(0);
            //TODO else if (name == "range") return new RangeProperty();               
            else if (name == "ref") return new ReferenceProperty();
            else
            {
                Debug.Assert(false, "IProperty::CreateFromTypeName: unknown type " + name);
                return null;
            }
        }
    }

    // Property class containing a value of type ValueType	
	public abstract class Property<T> : IProperty
	{
        protected T m_Value;        

        //! returns the value stored in this property
        public override object GetValue() { return m_Value; }	
    }
    
    //! Integer property class
	public class IntProperty : Property<int>
	{
        //! constructor
        public IntProperty(int value)
        {
            m_Value = value;
        }

        //! sets the value of this property from a string
        public override void SetValue(string strValue) { m_Value = int.Parse(strValue); }
	}

    //! Integer property class
    public class UIntProperty : Property<uint>
    {
        //! constructor
        public UIntProperty(uint value)
        {
            m_Value = value;
        }

        //! sets the value of this property from a string
        public override void SetValue(string strValue) { m_Value = System.UInt32.Parse(strValue, System.Globalization.NumberStyles.HexNumber); }
    }

    //! Float property class
    public class FloatProperty : Property<float>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue) { m_Value = float.Parse(strValue); }
    }

    //! String property class
    public class StringProperty : Property<string>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue) { m_Value = strValue; }
    }

    //! Bool property class
    public class BoolProperty : Property<bool>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue) { m_Value = (strValue == "1"); }
    }

    //! Vec2D property class
    public class Vec2DProperty : Property<Vector2>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue)
        {
            string[] tokens = strValue.Split();
            m_Value = new Vector2(float.Parse(tokens[0]), float.Parse(tokens[1]));            
        }
    }

    //! Vec3D property class
    public class Vec3DProperty : Property<Vector3>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue)
        {
            string[] tokens = strValue.Split();
            m_Value = new Vector3(float.Parse(tokens[0]), float.Parse(tokens[1]), float.Parse(tokens[2]));
        }
    }

    //! Struct property class
    public class StructProperty : IProperty
    {
        private PropertyStream m_Stream = new PropertyStream();

        //! returns the value stored in this property
        public override object GetValue() { return null; }

        //! sets the value of this property from a string
        public override void SetValue(string strValue) { }

        //! returns the property stream 
        public PropertyStream GetStream() { return m_Stream; }
    }

    //! user defined structs must inherit from this to become serializable
    public abstract class ISerializableStruct
	{
		//! Reads struct properties from a stream
        public abstract void Serialize(PropertyStream stream);
	};

    //! Array property class
    public class ArrayProperty : IProperty
    {
        private List<IProperty> m_aPropeties = new List<IProperty>();

        //! returns the value stored in this property
        public override object GetValue() { return null; }

        //! sets the value of this property from a string
        public override void SetValue(string strValue) { }

        //! returns the properties array
        public List<IProperty> GetProperties() { return m_aPropeties; }
    }

    //! Color property class
    public class ColorProperty : Property<Color>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue)
        {
            string[] tokens = strValue.Split();
            m_Value = new Color(float.Parse(tokens[0]), float.Parse(tokens[1]), float.Parse(tokens[2]));
        }
    }

    //! Bounding box property class
    public class BoundingBoxProperty : Property<BoundingBox>
    {
        //! sets the value of this property from a string
        public override void SetValue(string strValue)
        {
            string[] tokens = strValue.Split();
            switch (tokens.Length)
            {
                case 4:
                    m_Value = new BoundingBox(new Vector3(float.Parse(tokens[0]), float.Parse(tokens[1]), 0.0f),
                                              new Vector3(float.Parse(tokens[2]), float.Parse(tokens[3]), 0.0f));
                    break;

                case 6:
                    m_Value = new BoundingBox(new Vector3(float.Parse(tokens[0]), float.Parse(tokens[1]), float.Parse(tokens[2])),
                                              new Vector3(float.Parse(tokens[3]), float.Parse(tokens[4]), float.Parse(tokens[5])));
                    break;

                default:
                    Debug.Assert(false, "BoundinBoxProperty has invalid number of parameters");
                    break;
            }            
        }
    }

    //! Reference property class
    public class ReferenceProperty : StructProperty
    {
        private string m_ClassName;
        private string m_TemplatePath;

        public string ClassName
        {
            get { return m_ClassName; }
            set { m_ClassName = value; }
        }

        public string TemplatePath
        {
            get { return m_TemplatePath; }
            set { m_TemplatePath = value; }
        }
    }

    //! class to hold a reference to user data		
	public class Reference<T>
	{	
        private T m_Element = default(T);
		private bool m_bValid = false;	
		
		//! Sets the reference to one element
		public void Set(T element)
		{
			m_Element = element;
			m_bValid = true;
		}

		//! Returns a the contained element
		public T Get()	{ return m_Element; }

		//! returns true if the reference is valid
		public bool IsValid() { return m_bValid; }

		//! Explicitly clear the reference
		public void Clear()
		{
			m_bValid = false;
		}
	}
}

