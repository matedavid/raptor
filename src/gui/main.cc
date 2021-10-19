#include <iostream>
#include <gtkmm.h>

#include "browser.h"

int main(int argc, char* argv[])
{
	// Creates Gtk app
	auto app = Gtk::Application::create();

	// Create window
	auto window = Browser();

	return app->run(window, argc, argv);
}
