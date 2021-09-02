#pragma once

#include <wx/wx.h>
#include "browser.h"

class App : public wxApp
{
private:
	Browser* browser;
public:
	App();
	~App();

	virtual bool OnInit();
};
