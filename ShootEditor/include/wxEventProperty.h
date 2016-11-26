/* 

Amine Rehioui
Created: August 7th 2011

*/

#ifndef _WX_EVENT_PROPERTY_H_INCLUDED_
#define _WX_EVENT_PROPERTY_H_INCLUDED_

using namespace shoot;

// -----------------------------------------------------------------------
// wxEventProperty
// -----------------------------------------------------------------------
class wxEventProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxEventProperty)

public:

	//! constructor
    wxEventProperty(const wxString& name = wxPG_LABEL, const wxString& label = wxPG_LABEL, const wxString& value = wxEmptyString)
		: wxLongStringProperty(name, label, value)
	{
	}

	//! button click callback
    virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	//! sets the ID pointed at by this property
	void SetEventName(const std::string& strEventName) { m_strEventName = strEventName; }

	//! returns the ID pointed at by this property
	std::string GetEventName() const { return m_strEventName; }

private:

	std::string m_strEventName;
};

#endif // _WX_EVENT_PROPERTY_H_INCLUDED_

