/* 

Amine Rehioui
Created: May 28th 2010

*/

#ifndef _WX_ARRAY_PROPERTY_H_INCLUDED_
#define _WX_ARRAY_PROPERTY_H_INCLUDED_

using namespace shoot;

// forwards
namespace shoot
{	
	class PropertyList;
}

// -----------------------------------------------------------------------
// wxArrayProperty
// -----------------------------------------------------------------------
class wxArrayProperty : public wxStringProperty
{
	typedef wxStringProperty super;

public:

	//! event types
	enum E_Event
	{
		E_PropertyAdded,
		E_PropertyRemoved,
		E_SubPropertyChanged,
		E_None
	};

	wxArrayProperty( const wxString& label,	ArrayProperty* pProperty, PropertyList* pParent );

	virtual ~wxArrayProperty();

	//! set the event type
	inline void SetEventType(E_Event eEventType) { m_eEventType = eEventType; }

	//! returns the event type
	inline E_Event GetEventType() const { return m_eEventType; }

	//! sets the changed sub property index
	inline void SetChangedSubPropertyIndex(s32 index) { m_iChangedSubPropertyIndex = index; }

	//! returns the changed sub property index
	inline s32 GetChangedSubPropertyIndex() const { return m_iChangedSubPropertyIndex; }	

	//! returns the sub property template
	inline IProperty* GetSubPropertyTemplate() const { return m_pSubPropertyTemplate; }

	//! returns the struct template - in case the sub properties are struct, this is needed to initialize them
	inline ISerializableStruct* GetStructTemplate() { return m_pStructTemplate; }

	//! returns the max number of elements
	inline s32 GetMaxElems() const { return m_MaxElems; }

	//! fills the property with children - must be called after the array has been inserted to the property list
	void Fill(ArrayProperty* pProperty);

	//! updates the array property value
	void UpdateValue();

	//! returns the parent property list
	inline PropertyList* GetParent() { return m_pParent; }

private:

	IProperty* m_pSubPropertyTemplate;
	ISerializableStruct* m_pStructTemplate;
	IReference* m_pRefTemplate;
	ILink* m_pLinkTemplate;
	E_Event m_eEventType;
	s32 m_iChangedSubPropertyIndex;	
	s32 m_MaxElems;
	PropertyList* m_pParent;
};

//! wxArrayProperty editor
class wxArrayPropertyEditor : public wxPGTextCtrlEditor
{
    WX_PG_DECLARE_EDITOR_CLASS(wxArrayPropertyEditor)

public:

    wxArrayPropertyEditor() { }
    virtual ~wxArrayPropertyEditor() { }

    wxPG_DECLARE_CREATECONTROLS
    virtual bool OnEvent( wxPropertyGrid* propGrid,
                          wxPGProperty* property,
                          wxWindow* ctrl,
                          wxEvent& event ) const;
};

extern wxPGEditor* wxPGEditor_ArrayPropertyEditor;
extern wxPGEditor* wxPGConstructArrayPropertyEditorEditorClass();

#endif // _WX_ARRAY_PROPERTY_H_INCLUDED_

