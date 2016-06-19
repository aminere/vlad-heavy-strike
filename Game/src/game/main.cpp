/* 

Amine Rehioui
Created: February 21th 2010

*/

#include "ShootTest.h"

#include "GameEngine.h"
#include "AppSDL.h"

//! Entity registration function
extern void RegisterObjects();

int main(int argc, char** argv)
{
	{
		shoot::GraphicsDriver::E_DriverType eType = shoot::GraphicsDriver::DT_OpenGL;

#ifdef DX11
		eType = shoot::GraphicsDriver::DT_DirectX11;
#endif // DX11

		shoot::GameEngine engine(&RegisterObjects, eType);
		shoot::AppSDL app(shoot::Size(800, 600), false, true);

		engine.Init();

		app.Run();
	}

	return 0;
}

