/* 

Amine Rehioui
Created: April 4th 2010

*/

#include "MainFrame.h"

#include "EntityFactory.h"

#include "MemoryLeakCheck.h"

#include "EngineSDL.h"

//! Define this on the game side to register entities
extern void RegisterEntities();

//! The class that implements the program entry point
class Application : public wxApp
{
public:

    //! called during initialization
	/** return: true if succeeded, false to abort the application */
    virtual bool OnInit();

	//! called during un-initialization
    virtual int OnExit();

private:

	shoot::MainFrame* m_pMainFrame;
	shoot::Engine* m_pEngine;
};

//! Implement the main application methods
IMPLEMENT_APP(Application)

//! called during initialization
/** return: true if succeeded, false to abort the application */
bool Application::OnInit()
{    
	ENABLE_MEMORY_LEAK_CHECK

    if(!wxApp::OnInit())
	{
        return false;
	}

	// Init Factory and register entities
	shoot::EntityFactory::CreateInstance();
	RegisterEntities();

	// initialize the shoot engine
	m_pEngine = new shoot::EngineSDL(shoot::GraphicsDriver::DT_OpenGL, shoot::Size(640, 480), false);

    // create the main application window
    m_pMainFrame = new shoot::MainFrame(_T("Shoot Editor"));
    m_pMainFrame->Show();

    return true;
}

//! called during un-initialization
int Application::OnExit()
{
	shoot::EntityFactory::DestroyInstance();
	delete m_pEngine;

	return 0;
}

