#include "minimal.h"

#include "sillyUtils.h"
#include "sqliteWrapped\sqliteWrapped.h"

#include "lineMangler\AddSourceLine.h"
#include "lineMangler\QuixoteDatabase.h"

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
EVT_MENU(Minimal_GenerateCSV, MyFrame::OnAbout)
EVT_BUTTON(Minimal_AddNewString, MyFrame::OnAddNewString)
EVT_BUTTON(Minimal_UpdateString, MyFrame::OnUpdateString)
EVT_BUTTON(Minimal_PopulateDatabase, MyFrame::OnPopulateDatabase)
wxEND_EVENT_TABLE()

// frame constructor
MyFrame::MyFrame(const wxString& title, sqliteWrapped::Connection & connection)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(40, 40), wxSize(540, 1000)), m_connection(connection)
{
    // set the frame icon
    SetIcon(wxICON(sample));
    SetMenuBar(PopulateMenus());
	CreateStatusBar(2);
	SetStatusText("Ready");
	CreateLogger();

	m_notebook = new wxNotebook(this, wxID_ANY);

	auto firstPanel = new wxPanel(m_notebook, wxID_ANY);
	CreateAutoPopulatorPanel(firstPanel, 0);

	auto secondPanel = new wxPanel(m_notebook, wxID_ANY);
	CreateAddNewLinePanel(secondPanel, 0);

	auto lineHistoryPanel = new wxPanel(m_notebook, wxID_ANY);
	CreateAddNewLinePanel(lineHistoryPanel, 0);

	m_notebook->AddPage(firstPanel, "DB Populator");
	m_notebook->AddPage(secondPanel, "Add One line");
	m_notebook->AddPage(lineHistoryPanel, "Source History");

	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(m_notebook, 1, wxGROW);
	mainSizer->Add(m_log, 0, wxGROW);

	SetSizerAndFit(mainSizer);
}

MyFrame::~MyFrame()
{
	delete wxLog::SetActiveTarget(m_logOld);
}

void MyFrame::CreateAutoPopulatorPanel(wxPanel * parent, unsigned int nPanel, unsigned long style)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);
	m_boxSizers.push_back(sizer);

	parent->SetSizer(sizer);

	m_textControlNumberOfAddedLines = new wxTextCtrl(parent, wxID_ANY, wxT(""));

	wxBoxSizer *boxPopulateDB = new wxBoxSizer(wxHORIZONTAL);
	boxPopulateDB->Add(new wxStaticText(parent, wxID_ANY, wxT("# Lines")), 0, wxALL, 5);
	boxPopulateDB->Add(m_textControlNumberOfAddedLines, 0, wxALL, 5);
	boxPopulateDB->Add(new wxButton(parent, Minimal_PopulateDatabase, wxT("Populate")), 0, wxALL, 5);

	boxPopulateDB->GetItem((size_t)1)->SetProportion(1);

	sizer->Add(boxPopulateDB, wxSizerFlags().Expand().Border(wxALL, 5));
}

void MyFrame::CreateAddNewLinePanel(wxPanel* parent, unsigned int nPanel, unsigned long style)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);
	m_boxSizers.push_back(sizer);

	m_textControlLineText = new wxTextCtrl(parent, wxID_ANY, wxT("Text for new source line"));
	m_textControlStringID = new wxTextCtrl(parent, wxID_ANY, wxT("Text for String ID"));
	
	wxBoxSizer *boxTextInput = new wxBoxSizer(wxHORIZONTAL);
	boxTextInput->Add(m_textControlLineText, wxSizerFlags());
	boxTextInput->Add(m_textControlStringID, wxSizerFlags());

	boxTextInput->GetItem((size_t)0)->SetProportion(2);
	boxTextInput->GetItem((size_t)1)->SetProportion(1);

	sizer->Add(boxTextInput, wxSizerFlags().Expand().Border(wxALL, 5));

	wxBoxSizer *button_box = new wxBoxSizer(wxHORIZONTAL);
	button_box->Add(
		new wxButton(parent, Minimal_AddNewString, wxT("New Source Line")),
		wxSizerFlags().Border(wxALL, 5));
	button_box->Add(
		new wxButton(parent, Minimal_UpdateString, wxT("Update Source Line")),
		wxSizerFlags().Border(wxALL, 5));

	sizer->Add(button_box, wxSizerFlags().Right());

	parent->SetSizer(sizer);
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

// ---------------------------------------------------------------
// event handlers
// ---------------------------------------------------------------

void MyFrame::OnAddNewString(wxCommandEvent& WXUNUSED(event))
{
	// Fetch the data from the visual controls 
	wxLogMessage(m_textControlLineText->GetLineText(0));
	wxLogMessage(m_textControlStringID->GetLineText(0));
	
	lineMangler::AddSourceLine a (m_connection, 
		m_textControlStringID->GetLineText(0).ToStdString(), 
		m_textControlLineText->GetLineText(0).ToStdString(),
		1,	// Hardcoded Platform (Common by design) 
		0);	// Hardcoded Status

	if (!a.Commit())
		this->AddToLog(a.ErrorMessage());
}

void MyFrame::OnUpdateString(wxCommandEvent& WXUNUSED(event))
{
	// Fetch the data from the visual controls 
	wxLogMessage(m_textControlLineText->GetLineText(0));
	wxLogMessage(m_textControlStringID->GetLineText(0));

	lineMangler::ModifySourceLine a (m_connection,
		m_textControlStringID->GetLineText(0).ToStdString(),
		m_textControlLineText->GetLineText(0).ToStdString());

	if (!a.Commit())
		this->AddToLog(a.ErrorMessage());
}

void MyFrame::OnPopulateDatabase(wxCommandEvent& WXUNUSED(event))
{
	wxLogMessage("Populate the Database called " + m_textControlNumberOfAddedLines->GetLineText(0));
	try
	{
		int newLines = std::stoi(m_textControlNumberOfAddedLines->GetLineText(0).ToStdString());
		lineMangler::QuixoteDatabase qdb(m_connection, newLines, 5);
		if (!qdb.Commit())
			AddToLog(qdb.ErrorMessage());
	}
	catch (std::invalid_argument)
	{
		wxLogMessage("Error, please use a positive number");
	}
	wxLogMessage("Populate the Database finished");
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
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
