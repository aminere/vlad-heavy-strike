/* 

Amine Rehioui
Created: March 23rd 2011

*/

namespace shoot
{
    //! renderable entity with a material
    public class RenderableEntity : Entity
    {
		//! serializes the entity to/from a PropertyStream
		public override void Serialize(PropertyStream stream)
		{
			base.Serialize(stream);

            stream.SerializeReference("Material", MaterialRef);
		}

		//! reference to the material
        public Reference<Material> MaterialRef = new Reference<Material>();
	};
}

