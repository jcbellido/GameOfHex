#pragma once

#include <string>
#include <vector>

#include "wx/wxprec.h"
#include "wx/notebook.h"
#include "wx/dataview.h"
#include "wx/datetime.h"
#include "wx/splitter.h"

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
	Minimal_AddNewString = 1,
	Minimal_UpdateString = 2,
	Minimal_PopulateDatabase = 3,
	Minimal_GenerateCSV = wxID_ANY
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame, public ILogger
{
private:
	wxTextCtrl    *m_log;
	wxLog         *m_logOld;
	sqliteWrapped::Connection & m_connection;

private:
	wxTextCtrl * m_textControlLineText;
	wxTextCtrl * m_textControlStringID;
	wxTextCtrl * m_textControlNumberOfAddedLines;
	wxTextCtrl * m_textControlNumberOfVersionsPerLine;

	wxNotebook * m_notebook;
	
	std::vector<wxDataViewCtrl *> m_dataViews;
	std::vector<wxBoxSizer *> m_boxSizers;
	
public:
	// ctor(s)
	MyFrame(const wxString& title, sqliteWrapped::Connection & connection);
	
	virtual ~MyFrame();

	void AddToLog(const std::wstring & message);
	void AddToLog(const std::string & message);

private:
	void OnAbout(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAddNewString(wxCommandEvent& event);
	void OnUpdateString(wxCommandEvent& event);
	void OnPopulateDatabase(wxCommandEvent& event);

private:
	wxDECLARE_EVENT_TABLE();
	wxMenuBar * PopulateMenus();
	void CreateLogger();
	void CreateAddNewLinePanel(wxPanel* parent, unsigned int nPanel, unsigned long style = 0);
	void CreateAutoPopulatorPanel(wxPanel* parent, unsigned int nPanel, unsigned long style = 0);
};
