/* 

Amine Rehioui
Created: March 26th 2011

*/

#ifndef _WX_REFERENCE_PROPERTY_H_INCLUDED_
#define _WX_REFERENCE_PROPERTY_H_INCLUDED_

using namespace shoot;

// forwards
namespace shoot
{	
	class PropertyList;
}

// -----------------------------------------------------------------------
// wxReferenceProperty
// -----------------------------------------------------------------------
class wxReferenceProperty : public wxStringProperty
{
	typedef wxStringProperty super;

public:

	//! event types
	enum E_Event
	{
		E_PropertySetNew,
		E_PropertySetFromTemplate,
		E_PropertyCleared,
		E_SubPropertyChanged,
		E_PropertySaved,
		E_None
	};

	wxReferenceProperty( const wxString& label,	ReferenceProperty* pProperty, PropertyList* pParent );

	virtual ~wxReferenceProperty();

	//! set the event type
	void SetEventType(E_Event eEventType) { m_eEventType = eEventType; }

	//! returns the event type
	E_Event GetEventType() const { return m_eEventType; }

	//! Fills the property - must be called after the wx property has been inserted to the property list
	void Fill(ReferenceProperty* pProperty);

	//! updates the reference property value
	void UpdateValue(ReferenceProperty* pProperty);

	//! returns the parent property list
	PropertyList* GetParent() { return m_pParent; }

	//! sets the target class name 
	void SetClassName(std::string className) { m_ClassName = className; } 
	
	//! returns the target class name
	std::string GetClassName() const { return m_ClassName; }

	//! sets the template path
	void SetTemplatePath(const std::string& path) { m_strTemplatePath = path; }

	//! get the template path
	const std::string& GetTemplatePath() const { return m_strTemplatePath; }

private:
	
	PropertyList* m_pParent;
	E_Event m_eEventType;
	std::string m_ClassName;
	std::string m_strTemplatePath;
};

//! wxReferenceProperty editor
class wxReferencePropertyEditor : public wxPGTextCtrlEditor
{
	wxDECLARE_DYNAMIC_CLASS(wxReferencePropertyEditor);

public:

	wxPGWindowList CreateControls(wxPropertyGrid* propGrid,
		wxPGProperty* property,
		const wxPoint& pos,
		const wxSize& sz) const;

	virtual bool OnEvent(wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

#endif // _WX_REFERENCE_PROPERTY_H_INCLUDED_

