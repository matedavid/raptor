#include "browser.h"

Browser::Browser(const wxString& title)
				: wxFrame(NULL, wxID_ANY, title, wxPoint(40, 40), wxSize(1024, 640))
{
	m_text1 = new wxStaticText(this, wxID_ANY, wxT("Rendered Text!"));
}
