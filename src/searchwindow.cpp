#include "searchwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include <iostream>
#include <glibmm/i18n.h>

SearchWindow::SearchWindow( const std::shared_ptr<EpBox::Eyepiecebox>& epbox,
                            const std::shared_ptr<ScopeBox::Telescopebox>& scopebox) 
                            : m_epbox(epbox), m_scopebox(scopebox)
{    
    set_resizable(false);
    set_title("Search");
    epsearch.set_width_chars(40);
    scopesearch.set_width_chars(40);
    epsearchlabel.set_text(_("Eyepiece search: "));
    scopesearchlabel.set_text(_("Telescope search: "));
    casesensitivelabel.set_text(_("Match case: "));
    epsearchlabel.set_halign(Gtk::ALIGN_END);
    scopesearchlabel.set_halign(Gtk::ALIGN_END);
    casesensitivelabel.set_halign(Gtk::ALIGN_END);
    casesensitive = Gtk::make_managed<Gtk::Switch>();
    casesensitive->set_active(false);
    casesensitive->set_halign(Gtk::ALIGN_START);

    Uidefs::set_ui_spacing(grid);

    grid.attach(scopesearchlabel, 0, 0);
    grid.attach(scopesearch, 1, 0);
    grid.attach(epsearchlabel, 0, 1);
    grid.attach(epsearch, 1, 1);
    grid.attach(casesensitivelabel, 0, 2);
    grid.attach(*casesensitive, 1, 2);

    m_epbox->m_ecombomodel.set_ep_completion_model(epsearch);
    m_scopebox->m_scombomodel.set_scope_completion_model(scopesearch);

    casesensitive->property_active().signal_changed().connect([this]()
    {
      m_epbox->m_ecombomodel.set_case_sensitive(casesensitive->get_active());
    });

    casesensitive->property_active().signal_changed().connect([this]()
    {
      m_scopebox->m_scombomodel.set_case_sensitive(casesensitive->get_active());
    });

    add(grid);
    show_all_children();
}

bool SearchWindow::on_key_press_event(GdkEventKey* key_event)
{
  
  if(key_event->keyval == GDK_KEY_Escape)
  {
    hide();
    return true;
  }
  
  return Gtk::Window::on_key_press_event(key_event);
}
