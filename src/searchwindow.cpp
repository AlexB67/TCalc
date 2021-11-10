#include "searchwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/eventcontrollerkey.h>
#include <glibmm/i18n.h>

SearchWindow::SearchWindow(const std::shared_ptr<EpBox::Eyepiecebox> &epbox,
                           const std::shared_ptr<ScopeBox::Telescopebox> &scopebox)
    : m_epbox(epbox), m_scopebox(scopebox)
{
    set_resizable(false);
    set_hide_on_close(true);
    set_title(_("Search"));
    epsearch.set_width_chars(50);
    scopesearch.set_width_chars(50);
    epsearchlabel.set_text(_("Eyepiece search: "));
    scopesearchlabel.set_text(_("Telescope search: "));
    casesensitivelabel.set_text(_("Match case: "));
    epsearchlabel.set_halign(Gtk::Align::END);
    scopesearchlabel.set_halign(Gtk::Align::END);
    casesensitivelabel.set_halign(Gtk::Align::END);
    casesensitive = Gtk::make_managed<Gtk::Switch>();
    casesensitive->set_active(false);
    casesensitive->set_halign(Gtk::Align::START);

    Uidefs::set_ui_spacing(grid);

    grid.attach(scopesearchlabel, 0, 0);
    grid.attach(scopesearch, 1, 0);
    grid.attach(epsearchlabel, 0, 1);
    grid.attach(epsearch, 1, 1);
    grid.attach(casesensitivelabel, 0, 2);
    grid.attach(*casesensitive, 1, 2);

  //  m_epbox->m_ecombomodel.set_ep_completion_model(epsearch, epbox->m_emodelentry);
    m_scopebox->m_scombomodel.set_scope_completion_model(scopesearch);

    casesensitive->property_active().signal_changed().connect((sigc::track_obj([this]()
    { m_epbox->m_ecombomodel.set_case_sensitive(casesensitive->get_active()); }, *this)));

    casesensitive->property_active().signal_changed().connect((sigc::track_obj([this]()
    { m_scopebox->m_scombomodel.set_case_sensitive(casesensitive->get_active()); }, *this)));

    auto controller = Gtk::EventControllerKey::create();
    controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    controller->signal_key_pressed().connect(
    sigc::bind(sigc::mem_fun(*this, &SearchWindow::on_key_press_event), "capture"), false);
    add_controller(controller);

    set_child(grid);
}

bool SearchWindow::on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring&)
{
    if (keyval == GDK_KEY_Escape) hide();
    
    return false;
}
