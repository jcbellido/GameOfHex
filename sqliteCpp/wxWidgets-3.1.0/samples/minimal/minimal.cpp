#include "minimal.h"
#include "sillyUtils.h"

#include "connectionFactory.h"

#include "sqliteWrapped\sqliteWrapped.h"
#include "lineMangler\LineMangler.h"
#include "lineMangler\LineManglerGeneralFunctions.h"

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#else // !wxUSE_MENUS
    // If menus are not available add a button to access the about box
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
    aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
    sizer->Add(aboutBtn, wxSizerFlags().Center());
#endif // wxUSE_MENUS/!wxUSE_MENUS

	m_log = new wxTextCtrl(this, wxID_ANY, wxT("This is the log window.\n"),
		wxPoint(5, 260), wxSize(630, 100),
		wxTE_MULTILINE | wxTE_READONLY);

	m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));

	auto connection = lineMangler::ConnectionFactory::GetConnection(*this);

	// Let's bring it on ... 
	std::cout << "Reading the data files" << std::endl;
	lineMangler::LineLoader loader;
	loader.LoadFiles();

	// populate the data ...

	for (int i = lineMangler::LanguageCode::Arabic; i != lineMangler::LanguageCode::EndOfLanguageCode; i++)
	{
		auto lines = loader.GetLanguageLines(i);
		std::wstring message = L"Added language code " + std::to_wstring(i);
		this->AddToLog(message);
	}
	
	// like this .... 
#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Status default text");
#endif // wxUSE_STATUSBAR
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
