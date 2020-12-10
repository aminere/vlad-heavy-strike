/* 

Amine Rehioui
Created: April 4th 2010

*/

#include "Precompiled.h"

#include "MainFrame.h"

#include "EngineSDL.h"

#include "MemoryManager.h"

//! Define this on the game side to register entities
extern void RegisterObjects();

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
    if(!wxApp::OnInit())
	{
        return false;
	}

	// initialize the shoot engine
	m_pEngine = new shoot::EngineSDL(shoot::GraphicsDriver::DT_OpenGL, &RegisterObjects, shoot::Size(640, 480), false, 24, SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    // create the main application window
    m_pMainFrame = new shoot::MainFrame(_T("Shoot Editor"));
    m_pMainFrame->Show();

    return true;
}

//! called during un-initialization
int Application::OnExit()
{	
	delete m_pEngine;

	return 0;
}

