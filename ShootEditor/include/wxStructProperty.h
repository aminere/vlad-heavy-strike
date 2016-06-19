/* 

Amine Rehioui
Created: May 27th 2010

*/

#ifndef _WX_STRUCT_PROPERTY_H_INCLUDED_
#define _WX_STRUCT_PROPERTY_H_INCLUDED_

using namespace shoot;

// forwards
namespace shoot
{
	class PropertyList;
}

// -----------------------------------------------------------------------
// wxStructProperty
// -----------------------------------------------------------------------
class wxStructProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxStructProperty)

public:

    wxStructProperty(PropertyList* pParent = NULL,
					 const wxString& label = wxPG_LABEL, 
					 const wxString& name = wxPG_LABEL,
					 PropertyStream& stream = PropertyStream(SM_Write));

    virtual ~wxStructProperty()
	{		
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

//! struct needed by wxStructProperty to keep track of children updates
struct wxPGChildChangeInfo
{
	wxPGChildChangeInfo() : childIndex(-1)
	{
	}

	s32 childIndex;
	wxVariant childValue;
};

WX_PG_DECLARE_VARIANT_DATA(wxPGChildChangeInfoVariantData, wxPGChildChangeInfo, wxPG_NO_DECL)

#endif //_WX_STRUCT_PROPERTY_H_INCLUDED_

