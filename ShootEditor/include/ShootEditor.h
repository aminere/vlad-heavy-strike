/* 

Amine Rehioui
Created: April 10th 2010

*/

#ifndef _SHOOT_EDITOR_H_INCLUDED_
#define _SHOOT_EDITOR_H_INCLUDED_

namespace shoot
{	
	// forwards
	class MainFrame;
	class ViewPort;
	class TabContainer;
	class Path;

	/** Provides access to common editor operations,
		coordinates the communication between the controls */
	class ShootEditor : public Singleton<ShootEditor>
	{
		DECLARE_SINGLETON(ShootEditor);

	public:

		//! Initialize the controls
		virtual void Init(MainFrame* pFrame);

		//! Updates the editor
		void Update();

		//! Returns the tree view
		TreeView* GetTreeView() { return m_pTreeView; }

		//! returns the object inspector
		ObjectInspector* GetObjectInspector() { return m_pObjectInspector; }

		//! Returns a viewport
		ViewPort* GetViewPort();

		//! Returns the resource window
		ResourceWindow* GetResourceWindow() { return m_pResourceWindow; }

		//! returns the main tab control
		MainTabControl* GetMainTabControl() { return m_pMainTabControl; }

		//! Selects an entity
		Entity* SelectEntity(Entity* pEntity);

		//! Selects an entity
		virtual Entity* SelectEntity(Entity* pEntity, const Vector3& vRayStart, const Vector3& vRayDir);

		//! Returns the selected entity
		Entity* GetSelectedEntity();

		//! returns true if the engine is running
		bool IsEngineRunning() const { return m_bEngineStarted && !m_bEnginePaused; }

		//! returns true if the engine is started
		bool IsEngineStarted() const { return m_bEngineStarted; }
			
		//! update viewports sizes
		void UpdateViewPortsSizes();

		//! updates the camera selector
		void UpdateCameraSelectors();

		//! control modes
		enum E_ControlMode
		{
			ControlMode_Translate,
			ControlMode_Rotate,
			ControlMode_Scale
		};

		//! sets the control mode
		void SetControlMode(E_ControlMode eMode) { m_eControlMode = eMode; }

		//! returns pin status
		inline bool HasPin() const { return m_bHasPin; }

		//! sets pin status
		void SetHasPin(bool bSet);

		//! returns pin position
		inline const Vector3& GetPinPosition() const { return m_vPinPosition; }

		//! returns the control mode
		E_ControlMode GetControlMode() const { return m_eControlMode; } 

		//! starts the engine
		void StartEngine();

		//! stops the engine
		void StopEngine();

		//! pauses the engine
		void PauseEngine();

		//! returns a tab container
		TabContainer* GetTabContainer();

		//! returns a menu category for a class type
		virtual std::string GetMenuCategory(const char* classType);

		//! sets the delta time
		inline void SetDeltaTime(f32 fDeltaTime) { m_fDeltaTime = fDeltaTime; }

		//! sets the 3D control status
		inline void Set3DControl(bool b3DControl) { m_b3DControl = b3DControl; }

		//! gets the 3D control status
		inline bool Get3DControl() const { return m_b3DControl; }

		//! sets audio status
		void SetAudioEnabled(bool bEnabled);

		//! gets audio status
		inline bool IsAudioEnabled() const { return m_bAudioEnabled; }

		//! gets the locked entity
		inline Entity* GetLockedEntity() const { return m_LockedEntity; }

		//! sets the locked entity
		inline void SetLockedEntity(Entity* pEntity) { m_LockedEntity = pEntity; }

	protected:

		TreeView* m_pTreeView;		
		ObjectInspector* m_pObjectInspector;		
		ResourceWindow* m_pResourceWindow;
		MainTabControl* m_pMainTabControl;

		bool m_bEngineStarted;
		bool m_bEnginePaused;

		ShootEditorEventsHandler m_EditorEventsHandler;

		E_ControlMode m_eControlMode;
		bool m_bHasPin;
		Vector3 m_vPinPosition;
		bool m_b3DControl;
		bool m_bAudioEnabled;
		Handle<Entity> m_LockedEntity;

		f32 m_fDeltaTime;
	};
}

#endif // _SHOOT_EDITOR_H_INCLUDED_


