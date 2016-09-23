#include "interaction.h"

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = Context.device->getGUIEnvironment();

		switch (event.GUIEvent.EventType)
		{
		case EGET_SCROLL_BAR_CHANGED:
		{
			if (id == GUI_ID_TEST_COUNT_SCROLL_BAR)
			{
				Context.hexGUI->UpdateNumberOfTests();
				return true;
			}
			break;
		}
		case EGET_BUTTON_CLICKED:
			switch (id)
			{
			case GUI_ID_QUIT_BUTTON:
				Context.device->closeDevice();
				return true;
			case GUI_ID_RESET_GAME:
			{
				Context.board_model->ResetBoard();
				Context.board->ResetBoard();
				core::stringw status = "Board reseted";
				Context.board_status->setText(status.c_str());
				return true;
			}
			case GUI_ID_PLAYER_STARTS:
			{
				Context.board_model->ResetBoard();
				Context.board->ResetBoard();
				core::stringw status = "Waiting for Player move";
				Context.board_status->setText(status.c_str());
				Context.hexGUI->StartGameByHuman();
				return true;
			}
			case GUI_ID_CPU_STARTS:
			{
				Context.board_model->ResetBoard();
				Context.board->ResetBoard();
				core::stringw status = "Waiting for CPU move";
				Context.board_status->setText(status.c_str());
				Context.hexGUI->StartGameByCPU();
				return true;
			}
			default:
				return false;
			}
			break;

		default:
			break;
		}
	} 
	else if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if (event.MouseInput.Event == EMIE_LMOUSE_DOUBLE_CLICK)
		{
			if (Context.hexGUI->GetGameState() == GameStates::Waiting_Human)
			{
				line3d<f32> ray = collMan->getRayFromScreenCoordinates(Context.device->getCursorControl()->getPosition(), Context.smgr->getActiveCamera());
				core::vector3d<f32> intersection(-100, 0, 0);
				back_plane.getIntersectionWithLimitedLine(ray.start, ray.end, intersection);
				u32 output_id; 
				if (Context.board->ClickOnBoard(intersection, output_id))
				{
					if (Context.board_model->GetStoneColor(output_id) == CellState::Empty)
					{
						Context.board_model->ClickOnStone(output_id, Context.hexGUI->GetHumanColor());
						Context.board->UpdateFromModel();
						// FIXME move this logic to interaction.h ... close to CPU logic handling
						if (Context.board_model->CheckColorWins(Context.hexGUI->GetHumanColor()))
						{
							core::stringw status = "Player Wins";
							Context.board_status->setText(status.c_str());
							Context.hexGUI->HumanWins();
						}
						else
						{
							core::stringw status = "Waiting for CPU move";
							Context.board_status->setText(status.c_str());
							Context.hexGUI->EndOfHumanTurn();
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

// Deprecated most probably
void MyEventReceiver::UpdateBoardWinningLabel()
{
	auto redWins = Context.board_model->CheckColorWins(CellState::Red);
	if (redWins)
	{
		core::stringw status = "Player one (red) horizontal wins";
		Context.board_status->setText(status.c_str());
	}
	else
	{
		core::stringw status = "Player two (blue) vertical wins";
		Context.board_status->setText(status.c_str());
	}
}
