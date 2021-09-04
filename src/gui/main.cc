#include <iostream>
#include <gtkmm.h>

class Browser : public Gtk::Window
{
public:
	Browser();
};

Browser::Browser()
{
	set_title("Raptor Browser");
	set_default_size(1280, 960);
}

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create();
	auto window = Browser();

	return app->run(window);
}
