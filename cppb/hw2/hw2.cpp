#include <irrlicht.h>

#include "boardview.h"
#include "driverChoice.h"
#include "interaction.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


#include <iostream>

using namespace std;

int main()
{
	// Check if I need to change the type of project from Console into Window? 
	IrrlichtDevice *device =
		createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"The Game of Hex - Irrlicht based");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	// Useful for checking how clogged the system is right now
	auto fpsReference = guienv->addStaticText(L"FPS", rect<s32>(10, 30, 260, 48), true, true, 0, -1, true);

	BoardView boardView = BoardView(smgr, irr::core::vector3d<f32>(0, 0, 0), 11 ,1);

	auto board_center = boardView.GetBoardCenter();
	smgr->addCameraSceneNode(0, 
		vector3df(board_center.X, board_center.Y, - boardView.GetBoardWidth() * 0.85), 
		board_center);

	auto hg = HexGUI(device, guienv, &boardView, smgr);

	// collision plane

	int fps = -1;
	while (device->run())
	{
		if (device->isWindowActive())
		{

			driver->beginScene(true, true, SColor(255, 50, 51, 70));

			smgr->drawAll();

			int currentFps = driver->getFPS();
			if (fps != currentFps)
			{
				core::stringw strFps = "FPS: ";
				strFps += currentFps;
				fpsReference->setText(strFps.c_str());
				fps = currentFps;
			}

			guienv->drawAll();

			driver->endScene();
		}
		else
		{
			device->yield();
		}
	}

	device->drop();

	return 0;
}