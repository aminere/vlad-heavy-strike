/* 

Amine Rehioui
Created: April 16th 2011

*/

#ifndef _REFERENCE_H_INCLUDED_
#define _REFERENCE_H_INCLUDED_

namespace shoot
{
	// forwards
	class Object;

	//! Reference interface
	class IReference
	{
	public:

		//! constructor
		IReference();

		//! destructor
		virtual ~IReference()
		{
			Clear();
		}
		
		//! Sets the referenced object
		void SetObject(Object* pObject);

		//! Returns a pointer to the contained element
		inline Object* Get()
		{ 
			return m_pObject;
		}

		//! Returns a pointer to the contained element
		inline Object* Get() const
		{ 
			return m_pObject;
		}

		//! returns true if the reference is valid
		inline bool IsValid() const
		{
			return m_pObject != 0;
		}

		//! clears the reference
		inline void Clear()
		{
			SetObject(0);
		}

		//! returns the base class name
		virtual const char* GetClassName() const = 0;

	protected:

		Object* m_pObject;
	};

	//! class to hold a reference to an object
	template <class T>
	class Reference : public IReference
	{
	public:

		//! constructor
		Reference()
		{
		}		

		//! constructor
		Reference(T* pObject)
		{
			SetObject(pObject);
		}

		//! copy constructor
		Reference(const Reference<T>& other)
		{
			*this = other;
		}

		//! assignment operator
		Reference<T>& operator = (const Reference<T>& other)
		{			
			SetObject(other.m_pObject);
			return *this;
		}

		//! assignment operator
		Reference<T>& operator = (T* pObject)
		{
			SetObject(pObject);
			return *this;
		}

		//! comparison operator
		bool operator == (const Reference<T>& other) const
		{
			return (m_pObject == other.m_pObject);
		}

		//! comparison operator
		bool operator == (const T* pObject) const
		{
			return (m_pObject == pObject);
		}

		//! T* cast operator
		inline operator T*()
		{
			return static_cast<T*>(m_pObject);
		}

		//! const T* cast operator
		inline operator T*() const
		{
			return static_cast<T*>(m_pObject);
		}

		//! returns the class name
		const char* GetClassName() const
		{ 
			return T::GetStaticClassName();
		} 

		//! -> operator
		inline T* operator ->()
		{ 
			return static_cast<T*>(m_pObject);
		}

		//! const -> operator
		inline T* operator ->() const
		{
			return static_cast<T*>(m_pObject);
		}

		//! returns the object pointed at
		inline T* Get() 
		{ 
			return static_cast<T*>(m_pObject);
		}

		//! returns the object pointed at
		inline T* Get() const 
		{
			return static_cast<T*>(m_pObject);
		}
	};
}

#endif // _REFERENCE_H_INCLUDED_

