#ifndef BROWSER_H
#define BROWSER_H

#include <wx/wx.h>

class Browser : public wxFrame
{
public:
	Browser(const wxString& title);

	wxStaticText* m_text1 = nullptr;
};

#endif // BROWSER_H
