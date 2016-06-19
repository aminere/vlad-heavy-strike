/* 

Amine Rehioui
Created: April 4th 2010

*/

#include "ShootEditorCommon.h"
#include "ShootTestEditor.h"
#include "GameEngine.h"
#include "AppSDL.h"

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
	shoot::GraphicsDriver::E_DriverType eType = shoot::GraphicsDriver::DT_OpenGL;

#ifdef DX11
	eType = shoot::GraphicsDriver::DT_DirectX11;
#endif // DX11

	shoot::Engine* pEngine = new shoot::GameEngine(&RegisterObjects, eType);
	shoot::App* pApp = new shoot::AppSDL(shoot::Size(640, 480), false, false);

    // create the main application window
	shoot::MainFrame* pMainFrame = new shoot::MainFrame(_T("Shoot Editor"), wxSize(1024, 768), new shoot::ShootTestEditor());
    pMainFrame->Show();

    return true;
}

//! called during un-initialization
int Application::OnExit()
{
	delete shoot::Engine::Instance();
	delete shoot::App::Instance();

	return 0;
}

