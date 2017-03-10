#include "app.h"
#include "minimal.h"

#include "lineMangler\LineMangler.h"
#include "lineMangler\connectionFactory.h"

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	// Base connection ...

	m_connection = lineMangler::ConnectionFactory::GetConnection();
	
	lineMangler::LineLoader loader;
	loader.LoadFiles();

	// create the main application window
	MyFrame *frame = new MyFrame("SQLite with froth", m_connection);

	for (int i = lineMangler::LanguageCode::Arabic; i != lineMangler::LanguageCode::EndOfLanguageCode; i++)
	{
		auto lines = loader.GetLanguageLines(i);
		std::wstring message = L"Added language code " + std::to_wstring(i);
		frame->AddToLog(message);
	}

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	frame->Show(true);

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}
