#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
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

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10, 10, 260, 22), true);

	auto fpsReference = guienv->addStaticText(L"FPS", rect<s32>(10, 30, 260, 42), true);

	smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));

	int fps = -1;

	while (device->run())
	{
		if (device->isWindowActive())
		{

			driver->beginScene(true, true, SColor(255, 100, 101, 140));

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