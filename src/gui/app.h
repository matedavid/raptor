#ifndef APP_H
#define APP_H

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


#endif // APP_H
