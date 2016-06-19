/* 

Amine Rehioui
Created: August 7th 2010

*/

#ifndef _EDITOR_SETTINGS_H_INCLUDED_
#define _EDITOR_SETTINGS_H_INCLUDED_

namespace shoot
{
	//! class to keep track of editor settings
	class EditorSettings : public Entity
						 , public Singleton<EditorSettings>
	{
		DECLARE_SINGLETON(EditorSettings);
		DECLARE_OBJECT(EditorSettings, Entity);

	public:

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! window sizes
		enum E_WindowSize
		{
			WS_Stretch,
			WS_iPhone,
			WS_Count
		};

		static Size WindowSizes[WS_Count];

		//! Editor flags
		enum E_Flag
		{
			EF_RenderBBox =			0x1 << 0, // render bounding boxes
			EF_RenderEntity2DPos =	0x1 << 1, // render Entity2D positions
			EF_RenderEntity3DPos =	0x1 << 2, // render Entity3D positions
			EF_Render2DOrigin =		0x1 << 3, // renders the 2D world origin
			EF_Render3DOrigin =		0x1 << 4, // render 3D world origin
			EF_Render3DGrid =		0x1 << 5, // render 3D grid
			EF_CollisionShapes =	0x1 << 7, // render collision shapes
			EF_DebugIcons =			0x1 << 8, // render debug icons
			EF_RenderQuadTrees =	0x1 << 9, // render quad trees
			EF_RenderFrustums =		0x1 << 10, // render camera frustums
			EF_RenderPickingInfo =	0x1 << 11, // render 3D picking info
		};

		//! Returns flags status
		inline bool HasFlag(E_Flag eFlag) const { return (m_Flags.m_Mask & eFlag) != 0; }

		// public properties
		s32 WindowSize;

		// post rendering elements configuration
		Color BBoxColor;
		Color EntityPosColor;

		f32 fEntity2DPosSize;
		f32 f2DOriginSize;

		f32 fEntity3DPosSize;
		f32 f3DOriginSize;

		Color SelectedEntityColor;
		Color PathColor;
		Color CollisionShapeColor;

		f32 f2DZoomStep;
		f32 f3DZoomStep;
		f32 f3DStrafeStep;
		f32 f3DLookStep;

		f32 f3DZoomStepIncrFactor;
		f32 f3DStrafeStepIncrFactor;

		f32 fPathSmoothness;
		u32 GridSize;

		f32 fDebugIconSize;		
		f32 fPinSizeFactor;

		//! data structure for the flags
		struct Flags : public ISerializableStruct
		{
			//! constructor
			Flags() : m_Mask(0)
			{
			}

			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream);
			
			//! Sets rendering flags
			void Set(E_Flag eFlag, bool bEnabled = true) { bEnabled ? (m_Mask |= eFlag) : m_Mask &= ~eFlag; }

			u32 m_Mask;
		};

	private:

		// private properties
		Flags m_Flags;

		static const char* m_strSettingsFilePath;
	};
}

#endif // _EDITOR_SETTINGS_H_INCLUDED_


