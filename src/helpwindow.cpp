#include "helpwindow.hpp"
#include "gtkmmcustomutils.hpp"

HelpWindow::HelpWindow(const Glib::RefPtr<Gtk::Application> app)
{
    set_title("GNOME TCalc browser");
    const gchar* uri = "file:///home/afb/Documents/tcalc-help.html"; 
    help_page = std::make_unique<WebView>();

    Uidefs::set_ui_spacing(m_grid);
    m_entry.set_text(uri);
    m_frame.add(*help_page);
    m_entry.set_halign(Gtk::ALIGN_CENTER);
    m_entry.set_width_chars(60);
    help_page->set_vexpand(true);
    help_page->set_hexpand(true);
    
    m_grid.attach(m_entry, 0, 0); 
    m_grid.attach(m_frame, 0, 1);
    
    help_page->load_uri(uri);
    
    add(m_grid);
    
    show_all_children();
    
    auto window = app->get_windows();
    help_page->set_size_request(window[0]->get_width() / 2, window[0]->get_height() - 100);
    help_page->grab_focus();

    m_entry.signal_changed().connect([this](){
        m_entry.get_text();
        help_page->load_uri(m_entry.get_text().c_str());
    });
}

bool HelpWindow::on_key_press_event(GdkEventKey* key_event)
{  
  if(key_event->keyval == GDK_KEY_Escape)
  {
    hide();
    return true;
  }

  return Gtk::Window::on_key_press_event(key_event);
}