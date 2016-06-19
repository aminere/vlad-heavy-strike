/* 

Amine Rehioui
Created: December 10th 2010

*/

#ifndef _BONE_H_INCLUDED_
#define _BONE_H_INCLUDED_

namespace shoot
{
	//! bone class used by animated meshes
	class Bone
	{
	public:

		//! returns the bone name
		const std::string& GetName() const { return m_Name; }

		//! returns the bone transform
		const Matrix44& GetTransform() const { return m_Transform; }

		//! sets the bone transform
		void SetTransform(const Matrix44& pTransform) { m_Transform = pTransform; }		

	private:

		std::string m_Name;
		Matrix44 m_Transform;

		Bone* m_pParent;
		std::vector<Bone*> m_aChildren;		
	};
}

#endif // _BONE_H_INCLUDED_

