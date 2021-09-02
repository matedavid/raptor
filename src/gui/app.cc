#include "app.h"

IMPLEMENT_APP(App);

App::App()
{
}

App::~App()
{
}

bool App::OnInit()
{
	browser = new Browser(wxT("Raptor Browser"));
	browser->Show(true);
	return true;
}
