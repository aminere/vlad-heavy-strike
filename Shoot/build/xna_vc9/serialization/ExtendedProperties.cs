/* 

Amine Rehioui
Created: October 30th 2011

*/

namespace shoot
{
    //! Property interface
    /** A property is used to expose a variable to XML and to visual editors */
    public class ESP<T> : ISerializableStruct
    {
        //! ESP types
        public enum Type
        {
            Local,
            Current,
            Offset
        };

        //! Reads struct properties from a stream
        public override void Serialize(PropertyStream stream)
        {
            m_Value = stream.Serialize<T>("Value");
            m_Type = (Type)stream.Serialize<int>("Type");
        }

        //! Computes src and dest properties from source and dest extended properties
		public static void Compute(ESP<T> espSrc,
							       ESP<T> espDest,
							       out T src,
							       out T dest,							            
							       T current,
                                   T destOffset)
		{
			switch(espSrc.m_Type)
			{
			case Type.Local:
				src = espSrc.m_Value;
				break;

			case Type.Current:
                src = current;
				break;

			default:
				src = espSrc.m_Value;
                break;
			}

            switch (espDest.m_Type)
			{
                case Type.Local:
				dest = espDest.m_Value;
				break;

                case Type.Offset:
                dest = destOffset;
				break;

			default:
				dest = espDest.m_Value;
                break;
			}
		}

        T m_Value;
        Type m_Type = Type.Local;

        public T Value
        {
            get { return m_Value;  }
        }
    }
}

