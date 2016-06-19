/* 

Amine Rehioui
Created: August 24th 2011

*/

#ifndef _EDITOR_IDS_H_INCLUDED_
#define _EDITOR_IDS_H_INCLUDED_

namespace shoot
{
	// constants
	const u32 MaxEntityTypes = 1024;

	//! event IDs
	enum E_EventID
	{
		ID_Menu_New,
		ID_Menu_Open,
		ID_Menu_Save,
		ID_Menu_SaveAs,
		ID_Menu_ReloadLastSave,
		ID_Menu_Insert_FirstEntity = wxID_HIGHEST,
		ID_Menu_Insert_LastEntity = ID_Menu_Insert_FirstEntity + MaxEntityTypes,
		ID_Menu_Duplicate,
		ID_Menu_Remove,
		ID_Menu_ToggleGameCameraControl,
		ID_Menu_ShowSettings,
		ID_Menu_ShowConsole,
		ID_Edit_Translate,
		ID_Edit_Rotate,
		ID_Edit_Scale,
		ID_Edit_Pin,
		ID_Edit_Undo,
		ID_Edit_Redo,
		ID_Edit_UnlinkTemplate,
		ID_Edit_LockSelection,
		ID_Edit_UnlockSelection,
		ID_Engine_Start,
		ID_Engine_Stop,
		ID_Engine_Pause,
		ID_3DCameraSelector,
		ID_2DCameraSelector,
		ID_Menu_ToggleEditorPostRendering,
		ID_Menu_ToggleAudio,
		ID_Menu_Toggle3DControl,
		ID_ObjectInspector_Load,
		ID_ObjectInspector_Save,
		ID_ObjectInspector_SaveAs,
		ID_ObjectInspector_Reload,
		ID_ObjectInspector_AddComponent,
		ID_ObjectInspector_RemoveComponent,
		ID_ObjectInspector_CopyComponent,
		ID_ObjectInspector_PasteComponent,
		ID_ObjectInspector_SelectItem,
		ID_Property_MoveUp,
		ID_Property_MoveDown,
		ID_Property_Remove,
		ID_Property_Copy,
		//ID_Tab_New,
		ID_Tab_ResetView,
		ID_Tab_ClearFade,
		ID_Tab_ToggleBlackBars,
		ID_PropertyOverride,
		ID_ConsoleClear,
		ID_TreeView,
	};
}

#endif // _EDITOR_IDS_H_INCLUDED_

