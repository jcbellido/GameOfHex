#include "minimal.h"
#include "sillyUtils.h"
#include "sqliteWrapped\sqliteWrapped.h"

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
EVT_MENU(Minimal_GenerateCSV, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

// frame constructor
MyFrame::MyFrame(const wxString& title, sqliteWrapped::Connection & connection)
       : wxFrame(NULL, wxID_ANY, title), m_connection(connection)
{
    // set the frame icon
    SetIcon(wxICON(sample));

    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	fileMenu->Append(Minimal_GenerateCSV, "&Generate CSV\tAlt-V", "Generate a CSV, just for the lolz");
    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

	m_log = new wxTextCtrl(this, wxID_ANY, wxT("This is the log window.\n"),
		wxPoint(5, 260), wxSize(630, 100),
		wxTE_MULTILINE | wxTE_READONLY);

	m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
	
    CreateStatusBar(2);
    SetStatusText("Status default text");
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}

MyFrame::~MyFrame()
{
	delete wxLog::SetActiveTarget(m_logOld);
}

void MyFrame::AddToLog(const std::wstring &message)
{
	auto wx_message = wxFormatString(message.c_str());
	wxLogMessage(wx_message);
}

void MyFrame::AddToLog(const std::string &message)
{
	auto wideString = sillyUtils::ConvertToWide(message);
	auto wx_message = wxFormatString(message.c_str());
	wxLogMessage(wx_message);
}
