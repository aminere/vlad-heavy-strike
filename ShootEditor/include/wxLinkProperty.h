/* 

Amine Rehioui
Created: May 24th 2010

*/

#ifndef _WX_LINK_PROPERTY_H_INCLUDED_
#define _WX_LINK_PROPERTY_H_INCLUDED_

using namespace shoot;

// -----------------------------------------------------------------------
// wxLinkProperty
// -----------------------------------------------------------------------
class wxLinkProperty : public wxLongStringProperty
{
    WX_PG_DECLARE_DERIVED_PROPERTY_CLASS(wxLinkProperty)

public:

	//! constructor
    wxLinkProperty( const wxString& name = wxPG_LABEL, const wxString& label = wxPG_LABEL, const wxString& value = wxEmptyString);

	//! destructor
    virtual ~wxLinkProperty();

	//! button click callback
    virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

	//! is called after an the entity link has been changed
	/** return true to close the EntitySelector */
	static bool OnEntityLinkChanged(u32 entityID);

	//! returns the currently active link property
	static wxLinkProperty* GetCurrentProperty() { return ms_pCurrentProperty; }

	//! sets the ID pointed at by this property
	void SetLinkID(u32 ID) { m_LinkID = ID; }

	//! returns the ID pointed at by this property
	u32 GetLinkID() const { return m_LinkID; }

	//! sets the target base type
	void SetBaseType(const char* strBaseType) { m_strBaseType = strBaseType; }

    WX_PG_DECLARE_VALIDATOR_METHODS()

private:

	static wxLinkProperty* ms_pCurrentProperty;
	u32 m_LinkID;
	const char* m_strBaseType;
};

#endif // _WX_LINK_PROPERTY_H_INCLUDED_

