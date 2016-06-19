/* 

Amine Rehioui
Created: March 21th 2010

*/

#ifndef _IMAGE_H_INCLUDED_
#define _IMAGE_H_INCLUDED_

namespace shoot
{
	//! Image entity
	/** Ideal for still backgrounds and panels */
	class Image : public Entity2D				 
	{	
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Image, Entity2D);

		//! Constructor
		Image();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! returns the texture transformation matrix
		virtual Matrix44 GetTextureTransformationMatrix();

		//! updates the geometry
		void UpdateGeometry();

	protected:

		Matrix44 m_TextureTransform;
		bool m_bTextureTransformDirty;
	};
}

#endif // _IMAGE_H_INCLUDED_