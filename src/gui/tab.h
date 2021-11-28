#include <gtkmm.h>

#include "content.h"

class Tab : public Gtk::VBox
{
public:
  Gtk::Box m_toolbar;
  Gtk::Entry m_search_bar;
  Gtk::Button m_search_button;
  Gtk::Button m_new_tab_button;
  Content m_content;

private:
  void search_bar_button_pressed();

public:
  Tab();

};