/* 

Amine Rehioui
Created: April 11th 2010

*/

#ifndef _WX_COMMON_PROPERTIES_H_INCLUDED_
#define _WX_COMMON_PROPERTIES_H_INCLUDED_

using namespace shoot;

// -----------------------------------------------------------------------
// wxVector2Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(Vector2VariantData, Vector2, wxPG_NO_DECL)

class wxVector2Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxVector2Property)

public:

    wxVector2Property( const wxString& label = wxPG_LABEL, 
						const wxString& name = wxPG_LABEL,
						const Vector2& value = Vector2() );

    virtual ~wxVector2Property()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxVector3Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(Vector3VariantData, Vector3, wxPG_NO_DECL)

class wxVector3Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxVector3Property)

public:

    wxVector3Property( const wxString& label = wxPG_LABEL, 
						const wxString& name = wxPG_LABEL,
						const Vector3& value = Vector3::Zero );

    virtual ~wxVector3Property()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxVector4Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(Vector4VariantData, Vector4, wxPG_NO_DECL)

class wxVector4Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxVector4Property)

public:

    wxVector4Property( const wxString& label = wxPG_LABEL, 
						const wxString& name = wxPG_LABEL,
						const Vector4& value = Vector4::Zero );

    virtual ~wxVector4Property()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxPointProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(PointVariantData, Point, wxPG_NO_DECL)

class wxPointProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxPointProperty)

public:

    wxPointProperty( const wxString& label = wxPG_LABEL, 
					 const wxString& name = wxPG_LABEL,
					 const Point& value = Point() );

    virtual ~wxPointProperty()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxAABBox2DProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(AABBox2DVariantData, AABBox2D, wxPG_NO_DECL)

class wxAABBox2DProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxAABBox2DProperty)

public:

    wxAABBox2DProperty( const wxString& label = wxPG_LABEL, 
						const wxString& name = wxPG_LABEL,
						const AABBox2D& value = AABBox2D() );

    virtual ~wxAABBox2DProperty()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxAABBox3DProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(AABBox3DVariantData, AABBox3D, wxPG_NO_DECL)

class wxAABBox3DProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxAABBox3DProperty)

public:

    wxAABBox3DProperty( const wxString& label = wxPG_LABEL, 
						const wxString& name = wxPG_LABEL,
						const AABBox3D& value = AABBox3D() );

    virtual ~wxAABBox3DProperty()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxSizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(SizeVariantData, Size, wxPG_NO_DECL)

class wxSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxSizeProperty)

public:

    wxSizeProperty( const wxString& label = wxPG_LABEL, 
					const wxString& name = wxPG_LABEL,
					const Size& value = Size() );

    virtual ~wxSizeProperty()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

// -----------------------------------------------------------------------
// wxRangeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(RangeVariantData, Range, wxPG_NO_DECL)

class wxRangeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxRangeProperty)

public:

    wxRangeProperty( const wxString& label = wxPG_LABEL, 
					 const wxString& name = wxPG_LABEL,
					 const Range& value = Range() );

    virtual ~wxRangeProperty()
	{
	}

    WX_PG_DECLARE_PARENTAL_METHODS()
};

#endif // _WX_COMMON_PROPERTIES_H_INCLUDED_


