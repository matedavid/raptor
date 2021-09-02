#include "app.h"

#include "browser.h"

IMPLEMENT_APP(BrowserApp);

bool BrowserApp::OnInit()
{
	Browser* browser = new Browser(wxT("Raptor Browser"));
	browser->Show(true);
	return true;
}
