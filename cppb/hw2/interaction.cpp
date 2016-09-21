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
				Context.board->RandomizeBoard();
				return true;

			case GUI_ID_RESET_GAME:
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

	return false;
}
