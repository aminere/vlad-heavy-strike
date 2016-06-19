/* 

Amine Rehioui
Created: August 3rd 2011

*/

#ifndef _PATH_ELEMENT_H_INCLUDED_
#define _PATH_ELEMENT_H_INCLUDED_

namespace shoot
{
	//! Path Element entity	
	class PathElement : public Entity3D				    			   
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(PathElement, Entity3D);

		//! Constructor
		PathElement();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! returns the up vector
		inline const Vector3& GetUpVector() const { return m_vUpVector; }

		//! returns th follow duration
		inline f32 GetFollowDuration() const { return m_fFollowDuration; }

	private:

		// properties
		Vector3 m_vUpVector;
		f32 m_fFollowDuration;
	};
}

#endif // _PATH_ELEMENT_H_INCLUDED_

