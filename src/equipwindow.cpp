#include "equipwindow.hpp"
#include "appglobals.hpp"
#include <gtkmm/eventcontrollerkey.h>
#include <iostream>

EquipWindow::EquipWindow(const Glib::RefPtr<Gtk::Application> &app)
{
    set_resizable(false);
    set_hide_on_close(true);
    set_margin(Uidefs::BORDER_WIDTH);
    headerbar.set_show_title_buttons();
    headerbar.pack_start(switcher);
    headerlabel.set_markup(_("<b>Equipment editor</b>\n<sub>Add, remove or edit user equipment.</sub>"));
    headerbar.set_title_widget(headerlabel);
 
    set_titlebar(headerbar);
    // disable logging temporarily
    if (true == AppGlobals::LOGFLAG)
    {
        AppGlobals::LOGFLAG = false;
        m_previouslogstate = true;
    }

    stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

    epedit = std::make_unique<EpBox::EditEyepieces>(app, this);
    scopeedit = std::make_unique<ScopeBox::EditTelescopes>(app, this);

    scopegrid.attach(scopeedit->create_telescope_grid(), 0, 0); // create extra grid containers
    epgrid.attach(epedit->create_eyepiece_grid(), 0, 0);        // to keep GtkStack happy with child property_ids
    stack.add(scopegrid, "Edittelescopes", _("Edit telescopes"));
    stack.add(epgrid, "Editeyepieces", _("Edit eyepieces"));

    switcher.set_hexpand(false);
    stack.set_vexpand(true);
    stack.set_hexpand(true);
    switcher.set_stack(stack);

    set_child(stack);
    epedit->init();
    scopeedit->init();

    auto controller = Gtk::EventControllerKey::create();
    controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    controller->signal_key_pressed().connect(
        sigc::bind(sigc::mem_fun(*this, &EquipWindow::on_key_press_event), "capture"), false);
    add_controller(controller);

    signal_hide().connect((sigc::track_obj([this]()
    {
        if (true == m_previouslogstate)
            AppGlobals::LOGFLAG = true;
    }, *this)));
}

bool EquipWindow::on_key_press_event(guint keyval, guint, Gdk::ModifierType, const Glib::ustring &)
{
    if (keyval == GDK_KEY_Escape)
        hide();

    return false;
}