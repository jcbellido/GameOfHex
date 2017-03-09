#pragma once

#include <string>

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Logger.h"

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

#include "sqliteWrapped\sqliteWrapped.h"

// IDs for the controls and the menu commands
enum
{
	// menu items
	Minimal_Quit = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	Minimal_About = wxID_ABOUT,

	Minimal_GenerateCSV = wxID_ANY
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame, public ILogger
{
private:
	wxTextCtrl    *m_log;
	wxLog         *m_logOld;
	sqliteWrapped::Connection & m_connection;

public:
	// ctor(s)
	MyFrame(const wxString& title, sqliteWrapped::Connection & connection);

	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	virtual ~MyFrame();

	void AddToLog(const std::wstring & message);
	void AddToLog(const std::string & message);

private:
	// any class wishing to process wxWidgets events must use this macro
	wxDECLARE_EVENT_TABLE();
	wxMenuBar * PopulateMenus();
	void CreateLogger();
	void CreateAddNewLinePanel(wxBoxSizer * sizer);
};
