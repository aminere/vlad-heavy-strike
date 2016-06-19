/* 

Amine Rehioui
Created: May 29th 2010

*/

#ifndef _PROPERTY_LOCATOR_H_INCLUDED_
#define _PROPERTY_LOCATOR_H_INCLUDED_

namespace shoot
{
	//!PropertyLocator - Locates an IProperty given a wxProperty
	class PropertyLocator
	{
	public:

		//! finds a Property in a stream based on a wxProperty
		IProperty* GetProperty(wxPGProperty* pWxProperty, PropertyStream& stream);	

		//! finds a root Property in a stream based on a wxProperty
		IProperty* GetRootProperty(wxPGProperty* pWxProperty, PropertyStream& stream);	

	private:

		//! recursively fill wxProperty child traversal info (keeps track of indexes from property to root)
		void FillWxTraversalInfo(wxPGProperty* pWxProperty);

		//! based on the traversal info, locates a Property in the root stream
		IProperty* GetProperty(IProperty* pParent);

		std::vector<s32> m_aTraversalInfo;
	};
}

#endif // _PROPERTY_LOCATOR_H_INCLUDED_

