/* 

Amine Rehioui
Created: January 14th 2012

*/

#ifndef _COMPONENT_H_INCLUDED_
#define _COMPONENT_H_INCLUDED_

namespace shoot
{
	// forwards
	class Entity;

	//! Base Component class
	class Component : public Object
	{
		DECLARE_OBJECT(Component, Object);

	public:		

		//! constructor
		Component();

		//! destructor
		virtual ~Component();

		//! Initializes the component
		virtual void Init() { }

		//! Updates the component
		virtual void Update() { }

		//! sets the parent entity
		inline void SetParent(Entity* pParent) { m_pParent = pParent; }

		//! returns the parent entity
		inline Entity* GetParent() const { return m_pParent; }

	protected:

		Entity* m_pParent;
	};
}

#endif // _COMPONENT_H_INCLUDED_

