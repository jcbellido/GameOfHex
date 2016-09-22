#include "interaction.h"

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = Context.device->getGUIEnvironment();

		switch (event.GUIEvent.EventType)
		{

		case EGET_BUTTON_CLICKED:
			switch (id)
			{
			case GUI_ID_QUIT_BUTTON:
				Context.device->closeDevice();
				return true;

			case GUI_ID_RANDOMIZE_BOARD_BUTTON:
				Context.board_model->PopulateABoardAtRandom();
				Context.board->UpdateFromModel();
				return true;

			case GUI_ID_RESET_GAME:
				Context.board_model->ResetBoard();
				Context.board->ResetBoard();
				return true;

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
			// Just in case is the human players turn to move, then, accept the event
			line3d<f32> ray = collMan->getRayFromScreenCoordinates(Context.device->getCursorControl()->getPosition(), Context.smgr->getActiveCamera());
			core::vector3d<f32> intersection(-100, 0, 0);
			back_plane.getIntersectionWithLimitedLine(ray.start, ray.end, intersection);
			// Solucion temporal ... solo para determinar que el click sobre el tablero funciona
			Context.board->ClickOnBoard(intersection);
		}
	}

	return false;
}
