#pragma once

#include <wx/wx.h>

class Browser : public wxFrame
{
public:
	Browser(const wxString& title);

	wxStaticText* m_text1 = nullptr;
};
