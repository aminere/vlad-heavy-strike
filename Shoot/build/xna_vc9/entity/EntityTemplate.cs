/* 

Amine Rehioui
Created: March 23rd 2011

*/

namespace shoot
{
    //! Entity Template
    public class EntityTemplate : Entity
    {
		//! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
		{
			base.Serialize(stream);

            m_strTemplatePath = stream.Serialize<string>("TemplatePath");

            if (m_strTemplatePath.Length > 0)
            {
                LoadFromXML(m_strTemplatePath, this);
            }

            // serialize the instance data
            if(GetNumChildren() > 0)
            {
                m_InstanceData.m_Instance = GetChild(0);
                stream.SerializeStruct("InstanceData", m_InstanceData);
            }
		}

		//! structure to serialize the entity instance
        class InstanceData : ISerializableStruct
		{
			//! constructor
			public InstanceData()
			{
			}

			//! serialize the instance data
			public override void Serialize(PropertyStream stream)
            {
                m_Instance.Serialize(stream);
            }

			public Entity m_Instance;
		}

		// properties
		string m_strTemplatePath;
		InstanceData m_InstanceData = new InstanceData();
	}
}

