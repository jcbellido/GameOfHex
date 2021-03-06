#pragma once

#include <irrlicht.h>
#include "boardview.h"
#include "boardmodel.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class HexGUI;

struct SAppContext
{
	IrrlichtDevice *device;
	BoardView *board;
	ISceneManager* smgr;
	BoardModel *board_model;
	IGUIStaticText *board_status;
	HexGUI *hexGUI;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_RESET_GAME = 103,
	GUI_ID_EVALUATE_BOARD = 104,
	GUI_ID_PLAYER_STARTS = 105,
	GUI_ID_CPU_STARTS = 106,
	GUI_ID_TEST_COUNT_SCROLL_BAR = 107,
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

protected:
	void UpdateBoardWinningLabel();

private:
	SAppContext & Context;
	scene::ISceneCollisionManager* collMan = nullptr;
	core::plane3d<f32> back_plane;
};

class HexGUI
{
public:
	HexGUI(	IrrlichtDevice *device, 
			gui::IGUIEnvironment* guienv, 
			BoardView *boardView, 
			ISceneManager* smgr, 
			BoardModel *board_model): board_model(board_model)
	{
		IGUISkin* skin = guienv->getSkin();
		IGUIFont* font = guienv->getFont("../../irrlicht-1.8.3/media/fontlucida.png");
		if (font)
			skin->setFont(font);

		skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);

		guienv->addButton(rect<s32>(730, 560, 780, 560 + 26), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exits Program");

		guienv->addButton(rect<s32>(715, 30, 780, 30 + 26), 0, GUI_ID_RESET_GAME, L"Reset", L"Reset Game");
		
		guienv->addButton(rect<s32>(715, 90, 780, 90 + 26), 0, GUI_ID_PLAYER_STARTS, L"Player", L"Start game by player");

		guienv->addButton(rect<s32>(715, 120, 780, 120 + 26), 0, GUI_ID_CPU_STARTS, L"CPU", L"Start game by CPU");

		number_of_tests = guienv->addStaticText(L"Samples per move: 1000", rect<s32>(10, 500, 270, 500 + 18));
		scrollbar = guienv->addScrollBar(true, rect<s32>(10, 522, 270, 522 + 18), 0, GUI_ID_TEST_COUNT_SCROLL_BAR);
		scrollbar->setMin(1);
		scrollbar->setMax(10000);
		scrollbar->setPos(1000);
		board_model->SetTestsPerPosition(1000);

		auto static_text = guienv->addStaticText(L"Select starting player", rect<s32>(10, 30, 260, 48), true, true, 0, -1, true);
		board_status = static_text;

		// Store the appropriate data in a context structure.
		m_context.device = device;
		m_context.board = boardView;
		m_context.smgr = smgr;
		m_context.board_model = board_model;
		m_context.board_status = board_status;
		m_context.hexGUI = this;

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

public: 
	GameStates GetGameState() const { return current_state; }
	CellState GetHumanColor() const { return human_color; }
	CellState GetCPUColor() const { return cpu_color; }
	
	void HumanWins() { current_state = GameStates::Finished; }
	
	void CPUWins() { current_state = GameStates::Finished; }
	
	void EndOfHumanTurn() 
	{ 
		current_state = GameStates::Waiting_CPU; 
		m_context.board_model->ComputeMove(m_context.hexGUI->GetCPUColor());
	}
	
	void EndOfCPUTurn() 
	{ 
		m_context.board->UpdateFromModel();
		current_state = GameStates::Waiting_Human; 
		if (m_context.board_model->CheckColorWins(m_context.hexGUI->GetCPUColor()))
		{
			core::stringw status = "CPU Wins";
			m_context.board_status->setText(status.c_str());
			m_context.hexGUI->CPUWins();
		}
		else
		{
			core::stringw status = "Waiting for Player move";
			m_context.board_status->setText(status.c_str());
		}
	}

	void UpdateNumberOfTests()
	{
		s32 pos = scrollbar->getPos();
		core::stringw status = "Samples per move: ";
		status += pos;
		number_of_tests->setText(status.c_str());
		board_model->SetTestsPerPosition(static_cast<unsigned int>(pos));
	}

public: 
	void StartGameByHuman() 
	{ 
		current_state = GameStates::Waiting_Human; 
		human_color = CellState::Red;
		cpu_color = CellState::Blue;
	}

	void StartGameByCPU() 
	{ 
		current_state = GameStates::Waiting_CPU; 
		human_color = CellState::Blue;
		cpu_color = CellState::Red;
		m_context.board_model->ComputeMove(m_context.hexGUI->GetCPUColor());
	}

protected: 
	SAppContext m_context;
	MyEventReceiver *m_receiver = nullptr;
	GameStates current_state = GameStates::Not_Started;
	CellState human_color = CellState::Empty;
	CellState cpu_color = CellState::Empty;
	BoardModel *board_model;
	IGUIStaticText *board_status;
	IGUIStaticText *number_of_tests;
	IGUIScrollBar* scrollbar;
};
