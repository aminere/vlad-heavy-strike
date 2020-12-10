/* 

Amine Rehioui
Created: July 7th 2012

*/

#ifndef _ARRAY_H_INCLUDED_
#define _ARRAY_H_INCLUDED_

namespace shoot
{
	//! Serializable Array interface
	class IArray
	{
	public:	
		//! Allocates one element, calls the approriate constructor
		virtual void* Alloc() = 0;
		
		//! Adds one element, the pointer is cast to the appropriate type and the result is copied into the array
		virtual void Add(void* pElement) = 0;

		//! Returns a pointer to an element, the properties are responsible for casting to the appropriate type
		virtual void* GetPtr(size_t index) = 0;

		//! Returns the number of elements
		virtual size_t GetSize() const = 0;

		//! Explicitly clear the array
		virtual void Clear() = 0;

		//! removes one element from the array
		virtual void* Remove(size_t index) = 0;

		//! deletes one element from the array
		virtual void Delete(size_t index) = 0;

		//! inserts one element in the array
		virtual void Insert(size_t index, void* pElement) = 0;
	};

	//! Concrete array class to be used to expose arrays of user data
	template <class T>
	class Array : public IArray
	{
	public:

		//! constructor
		Array()
		{
		}

		//! copy constructor
		Array(const Array<T>& other)
		{
			*this = other;
		}

		//! destructor
		~Array()
		{
			Clear();
		}

		//! returns a specific element
		T& operator[](size_t index) { return *aElements[index]; }

		//! returns a specific element
		const T& operator[](size_t index) const { return *aElements[index]; }

		//! Allocates one element, calls the approriate constructor
		void* Alloc() {	return snew T(); }		

		//! Adds one element, the pointer is cast to the appropriate type and the result is copied into the array
		void Add(void* pElement)
		{			
			aElements.push_back(static_cast<T*>(pElement));
		}

		//! Returns a pointer to an element, the properties are responsible for casting to the appropriate type
		void* GetPtr(size_t index)	{ return aElements[index]; }

		//! Returns the number of elements
		size_t GetSize() const { return aElements.size(); }

		//! Explicitly clear the array
		void Clear() 
		{ 
			for(auto i=0; i<aElements.size(); ++i)
			{
				delete aElements[i];
			}
			aElements.clear();
		}

		//! removes one element from the array
		void* Remove(size_t index)
		{
			void* pElement = aElements[index];
			aElements.erase(aElements.begin()+index);
			return pElement;
		}

		//! deletes one element from the array
		void Delete(size_t index)
		{
			delete aElements[index];
			aElements.erase(aElements.begin()+index);
		}

		//! inserts one element in the array
		void Insert(size_t index, void* pElement)
		{
			aElements.insert(aElements.begin()+index, static_cast<T*>(pElement));
		}
			
		//! comparison operator
		bool operator == (const Array<T>& other) const
		{
			if(aElements.size() == other.aElements.size())
			{
				for(u32 i=0; i<aElements.size(); ++i)
				{
					if(!(*(aElements[i]) == *(other.aElements[i])))
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		//! assignment operator
		Array<T>& operator = (const Array<T>& other)
		{
			Clear();
			for(u32 i=0; i<other.GetSize(); ++i)
			{
				T* pElem = static_cast<T*>(Alloc());
				*pElem = other[i];
				Add(pElem);
			}
			return *this;
		}

	private:

		std::vector<T*> aElements;
	};
}

#endif // _ARRAY_H_INCLUDED_

