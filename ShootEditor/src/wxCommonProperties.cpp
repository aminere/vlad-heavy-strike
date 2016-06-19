/* 

Amine Rehioui
Created: April 11th 2010

*/

#include "ShootEditorCommon.h"

#include "wxCommonProperties.h"

#include "EntitySelector.h"

// -----------------------------------------------------------------------
// wxVector2Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(Vector2VariantData, Vector2)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector2Property, wxPGProperty, Vector2, const Vector2&, TextCtrl)

wxVector2Property::wxVector2Property(const wxString& label, const wxString& name, const Vector2& value) 
: wxPGProperty(label, name)
{
    SetValue(Vector2ToVariant(value));
    AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.X));
    AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.Y));	
}

void wxVector2Property::RefreshChildren()
{
    if (GetCount())
	{	
		Vector2& vec = Vector2FromVariant(m_value);
		Item(0)->SetValue(vec.X);
		Item(1)->SetValue(vec.Y);
	}    
}

void wxVector2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Vector2& vec = Vector2FromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: vec.X = childValue.GetDouble(); break;
        case 1: vec.Y = childValue.GetDouble(); break;
    }
}

// -----------------------------------------------------------------------
// wxVector3Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(Vector3VariantData, Vector3)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector3Property, wxPGProperty, Vector3, const Vector3&, TextCtrl)

wxVector3Property::wxVector3Property(const wxString& label, const wxString& name, const Vector3& value) 
: wxPGProperty(label, name)
{
    SetValue(Vector3ToVariant(value));
    AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.X));
    AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.Y));	
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.Z));	
}

void wxVector3Property::RefreshChildren()
{
    if (GetCount())
	{	
		Vector3& vec = Vector3FromVariant(m_value);
		Item(0)->SetValue(vec.X);
		Item(1)->SetValue(vec.Y);
		Item(2)->SetValue(vec.Z);
	}    
}

void wxVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Vector3& vec = Vector3FromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: vec.X = childValue.GetDouble(); break;
        case 1: vec.Y = childValue.GetDouble(); break;
		case 2: vec.Z = childValue.GetDouble(); break;
    }
}

// -----------------------------------------------------------------------
// wxVector4Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(Vector4VariantData, Vector4)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector4Property, wxPGProperty, Vector4, const Vector4&, TextCtrl)

wxVector4Property::wxVector4Property(const wxString& label, const wxString& name, const Vector4& value) 
: wxPGProperty(label, name)
{
    SetValue(Vector4ToVariant(value));
    AddPrivateChild(new wxFloatProperty(wxT("X"), wxPG_LABEL, value.X));
    AddPrivateChild(new wxFloatProperty(wxT("Y"), wxPG_LABEL, value.Y));
	AddPrivateChild(new wxFloatProperty(wxT("Z"), wxPG_LABEL, value.Z));
	AddPrivateChild(new wxFloatProperty(wxT("W"), wxPG_LABEL, value.W));
}

void wxVector4Property::RefreshChildren()
{
    if (GetCount())
	{	
		Vector4& vec = Vector4FromVariant(m_value);
		Item(0)->SetValue(vec.X);
		Item(1)->SetValue(vec.Y);
		Item(2)->SetValue(vec.Z);
		Item(3)->SetValue(vec.W);
	}    
}

void wxVector4Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Vector4& vec = Vector4FromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: vec.X = childValue.GetDouble(); break;
        case 1: vec.Y = childValue.GetDouble(); break;
		case 2: vec.Z = childValue.GetDouble(); break;
		case 3: vec.W = childValue.GetDouble(); break;
    }
}

// -----------------------------------------------------------------------
// wxPointProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(PointVariantData, Point)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxPointProperty, wxPGProperty, Point, const Point&, TextCtrl)

wxPointProperty::wxPointProperty(const wxString& label, const wxString& name, const Point& value) 
: wxPGProperty(label, name)
{
    SetValue(PointToVariant(value));
    AddPrivateChild( new wxIntProperty(wxT("X"), wxPG_LABEL, value.X) );
    AddPrivateChild( new wxIntProperty(wxT("Y"), wxPG_LABEL, value.Y) );
}

void wxPointProperty::RefreshChildren()
{
    if (GetCount())
	{	
		Point& point = PointFromVariant(m_value);
		Item(0)->SetValue( (long)point.X );
		Item(1)->SetValue( (long)point.Y );
	}    
}

void wxPointProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Point& point = PointFromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: point.X = childValue.GetInteger(); break;
        case 1: point.Y = childValue.GetInteger(); break;
    }
}

// -----------------------------------------------------------------------
// wxAABBox2DProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(AABBox2DVariantData, AABBox2D)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxAABBox2DProperty, wxPGProperty, AABBox2D, const AABBox2D&, TextCtrl)

wxAABBox2DProperty::wxAABBox2DProperty(const wxString& label, const wxString& name, const AABBox2D& value) 
: wxPGProperty(label, name)
{
    SetValue(AABBox2DToVariant(value));
    AddPrivateChild( new wxVector2Property(wxT("Min"), wxPG_LABEL, value.Min()) );
    AddPrivateChild( new wxVector2Property(wxT("Max"), wxPG_LABEL, value.Max()) );
}

void wxAABBox2DProperty::RefreshChildren()
{
    if (GetCount())
	{	
		AABBox2D& box = AABBox2DFromVariant(m_value);
		Item(0)->SetValue(Vector2ToVariant(box.Min()));
		Item(1)->SetValue(Vector2ToVariant(box.Max()));
	}    
}

void wxAABBox2DProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    AABBox2D& box = AABBox2DFromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: box.Set(Vector2FromVariant(childValue), box.Max()); break;
		case 1: box.Set(box.Min(), Vector2FromVariant(childValue)); break;
    }
}

// -----------------------------------------------------------------------
// wxAABBox3DProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(AABBox3DVariantData, AABBox3D)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxAABBox3DProperty, wxPGProperty, AABBox3D, const AABBox3D&, TextCtrl)

wxAABBox3DProperty::wxAABBox3DProperty(const wxString& label, const wxString& name, const AABBox3D& value) 
: wxPGProperty(label, name)
{
    SetValue(AABBox3DToVariant(value));
    AddPrivateChild( new wxVector3Property(wxT("Min"), wxPG_LABEL, value.Min()) );
    AddPrivateChild( new wxVector3Property(wxT("Max"), wxPG_LABEL, value.Max()) );
}

void wxAABBox3DProperty::RefreshChildren()
{
    if (GetCount())
	{	
		AABBox3D& box = AABBox3DFromVariant(m_value);
		Item(0)->SetValue(Vector3ToVariant(box.Min()));
		Item(1)->SetValue(Vector3ToVariant(box.Max()));
	}    
}

void wxAABBox3DProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    AABBox3D& box = AABBox3DFromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: box.Set(Vector3FromVariant(childValue), box.Max()); break;
		case 1: box.Set(box.Min(), Vector3FromVariant(childValue)); break;
    }
}

// -----------------------------------------------------------------------
// wxSizeProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(SizeVariantData, Size)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxSizeProperty, wxPGProperty, Size, const Size&, TextCtrl)

wxSizeProperty::wxSizeProperty(const wxString& label, const wxString& name, const Size& value) 
: wxPGProperty(label, name)
{
    SetValue(SizeToVariant(value));
    AddPrivateChild( new wxIntProperty(wxT("Width"), wxPG_LABEL, value.Width) );
    AddPrivateChild( new wxIntProperty(wxT("Height"), wxPG_LABEL, value.Height) );
}

void wxSizeProperty::RefreshChildren()
{
    if (GetCount())
	{	
		Size& size = SizeFromVariant(m_value);
		Item(0)->SetValue( (long)size.Width );
		Item(1)->SetValue( (long)size.Height );
	}    
}

void wxSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Size& size = SizeFromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: size.Width = childValue.GetInteger(); break;
        case 1: size.Height = childValue.GetInteger(); break;
    }
}

// -----------------------------------------------------------------------
// wxRangeProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA(RangeVariantData, Range)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxRangeProperty, wxPGProperty, Range, const Range&, TextCtrl)

wxRangeProperty::wxRangeProperty(const wxString& label, const wxString& name, const Range& value) 
: wxPGProperty(label, name)
{
    SetValue(RangeToVariant(value));
    AddPrivateChild( new wxFloatProperty(wxT("Min"), wxPG_LABEL, value.Min) );
    AddPrivateChild( new wxFloatProperty(wxT("Max"), wxPG_LABEL, value.Max) );
}

void wxRangeProperty::RefreshChildren()
{
    if (GetCount())
	{	
		Range& range = RangeFromVariant(m_value);
		Item(0)->SetValue(range.Min);
		Item(1)->SetValue(range.Max);
	}    
}

void wxRangeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{	
    Range& range = RangeFromVariant(thisValue);
    switch ( childIndex )
    {
		case 0: range.Min = childValue.GetDouble(); break;
        case 1: range.Max = childValue.GetDouble(); break;
    }
}
