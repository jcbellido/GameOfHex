#pragma once

#include <string>

#include "wx/wxprec.h"

#include "sqliteWrapped\sqliteWrapped.h"
#include "lineMangler\LineMangler.h"
#include "lineMangler\LineManglerGeneralFunctions.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Logger.h"

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif


// IDs for the controls and the menu commands
enum
{
	// menu items
	Minimal_Quit = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	Minimal_About = wxID_ABOUT
};


// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
	// override base class virtuals
	// ----------------------------

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit() wxOVERRIDE;
};


// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame, public ILogger
{
private:
	wxTextCtrl    *m_log;
	wxLog         *m_logOld;
public:
	// ctor(s)
	MyFrame(const wxString& title);

	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	virtual ~MyFrame();

	void AddToLog(const std::wstring & message);
	void AddToLog(const std::string & message);

private:
	// any class wishing to process wxWidgets events must use this macro
	wxDECLARE_EVENT_TABLE();
};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(MyApp);

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()