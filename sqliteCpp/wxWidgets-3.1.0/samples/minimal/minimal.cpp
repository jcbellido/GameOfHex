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
    SetMenuBar(PopulateMenus());
	auto boxSizerCentral = new wxBoxSizer( wxVERTICAL );

	CreateAddNewLinePanel(boxSizerCentral);

	CreateLogger();
	boxSizerCentral->Add(m_log, wxSizerFlags(1).Expand().Border(wxALL, 5));

	// Central Stack Panel (vertical box sizer central) added to the mix
	SetSizer(boxSizerCentral);

    CreateStatusBar(2);
    SetStatusText("Ready");

	boxSizerCentral->SetSizeHints(this);
}

void MyFrame::CreateAddNewLinePanel(wxBoxSizer * sizer)
{
	sizer->Add(
		new wxStaticText(this, wxID_ANY, wxT("Add a new line to the mix")),
		wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL & ~wxBOTTOM, 5));

	sizer->Add(
		new wxTextCtrl(this, wxID_ANY, wxT("Text for new source line"), wxDefaultPosition, wxSize(10, 20)),
		wxSizerFlags().Expand().Border(wxALL, 5));

	wxBoxSizer *button_box = new wxBoxSizer(wxHORIZONTAL);
	button_box->Add(
		new wxButton(this, wxID_ANY, wxT("New Source Line")),
		wxSizerFlags().Border(wxALL, 5));
	button_box->Add(
		new wxButton(this, wxID_ANY, wxT("No Op")),
		wxSizerFlags().Border(wxALL, 5));

	sizer->Add(button_box, wxSizerFlags().Right());
}

void MyFrame::CreateLogger()
{
	m_log = new wxTextCtrl(this, wxID_ANY, wxT("This is the log window.\n"),
		wxDefaultPosition, wxSize(300, 100), wxTE_MULTILINE | wxTE_READONLY); 

	m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
}

wxMenuBar * MyFrame::PopulateMenus()
{
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
	
	return menuBar;
}


// ------------------------------------------------------------------------------------
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
