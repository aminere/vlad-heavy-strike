/* 

Amine Rehioui
Created: May 27th 2010

*/

#include "ShootEditorCommon.h"

#include "wxStructProperty.h"

#include "PropertyList.h"

// -----------------------------------------------------------------------
// wxStructProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(wxPGChildChangeInfoVariantData, wxPGChildChangeInfo)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxStructProperty, wxPGProperty, wxPGChildChangeInfo, const wxPGChildChangeInfo&, TextCtrl)

wxStructProperty::wxStructProperty(PropertyList* pParent, const wxString& label, const wxString& name, PropertyStream& stream) 
: wxPGProperty(label, name)
{
	wxPGChildChangeInfo childChangeInfo;
	SetValue(wxPGChildChangeInfoToVariant(childChangeInfo));

	for(u32 i=0; i<stream.GetNumProperties(); ++i)
	{
		IProperty* pProperty = stream.GetProperty(i);
		wxPGProperty* pWxProperty = PropertyList::GetWxProperty(pProperty, pParent);
		if(pWxProperty)
		{
			AddPrivateChild(pWxProperty);

			bool bParentDisabled = !GetGrid()->IsPropertyEnabled(this);
			if(bParentDisabled)
			{
				GetGrid()->DisableProperty(pWxProperty);
			}
		}
	}
}

void wxStructProperty::RefreshChildren()
{
	wxPGChildChangeInfo& childChangeInfo = wxPGChildChangeInfoFromVariant(m_value);
	Item(childChangeInfo.childIndex)->SetValue(childChangeInfo.childValue);
}

void wxStructProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
	wxPGChildChangeInfo& childChangeInfo = wxPGChildChangeInfoFromVariant(thisValue);
	childChangeInfo.childIndex = childIndex;
	childChangeInfo.childValue = childValue;
}

