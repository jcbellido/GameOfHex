#pragma once

#include <irrlicht.h>
#include "boardview.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct SAppContext
{
	IrrlichtDevice *device;
	BoardView *board;
	ISceneManager* smgr;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_RANDOMIZE_BOARD_BUTTON = 102,
	GUI_ID_RESET_GAME = 103,
};

class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context)
	{ 
		collMan = context.smgr->getSceneCollisionManager();
		back_plane = core::plane3d<f32>(core::vector3d<f32>(0, 0, 0), core::vector3d<f32>(0, 0, -1));
	}

	virtual bool OnEvent(const SEvent& event);

private:
	SAppContext & Context;
	scene::ISceneCollisionManager* collMan = nullptr;
	core::plane3d<f32> back_plane;
};

class HexGUI
{
public:
	HexGUI(IrrlichtDevice *device, gui::IGUIEnvironment* guienv, BoardView *boardView, ISceneManager* smgr)
	{
		IGUISkin* skin = guienv->getSkin();
		IGUIFont* font = guienv->getFont("../../irrlicht-1.8.3/media/fontlucida.png");
		if (font)
			skin->setFont(font);

		skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

		guienv->addButton(rect<s32>(30, 560, 130, 560 + 26), 0, GUI_ID_RANDOMIZE_BOARD_BUTTON, L"Randomize", L"Randomize board (just graphical)");

		guienv->addButton(rect<s32>(730, 560, 780, 560 + 26), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exits Program");

		guienv->addButton(rect<s32>(730, 30, 780, 30 + 26), 0, GUI_ID_RESET_GAME, L"Reset", L"Reset Game");

		// Store the appropriate data in a context structure.
		m_context.device = device;
		m_context.board = boardView;
		m_context.smgr = smgr;

		// Then create the event receiver, giving it that context structure.
		m_receiver = new MyEventReceiver(m_context);

		// And tell the device to use our custom event receiver.
		device->setEventReceiver(m_receiver);
	}
	
	~HexGUI()
	{
		if (m_receiver != nullptr)
		{
			delete(m_receiver);
		}
	}

protected: 
	SAppContext m_context;
	MyEventReceiver *m_receiver = nullptr;
};

//struct Interaction
//{
//
//};