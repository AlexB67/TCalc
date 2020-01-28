#include "equipwindow.hpp"
#include <iostream>
#include "appglobals.hpp"

EquipWindow::EquipWindow(const Glib::RefPtr<Gtk::Application> &app)
{
  set_resizable(false);
  set_border_width(Uidefs::BORDER_WIDTH);
  headerbar.set_show_close_button();
  headerbar.add(switcher);
  headerbar.set_title(_("Equipment editor"));
  headerbar.set_subtitle("Edit user equipment");
  set_titlebar(headerbar);
  
  if (true == AppGlobals::LOGFLAG) 
  {
    AppGlobals::LOGFLAG = false;
    m_previouslogstate = true;
  }

  stack.set_transition_type(Gtk::StackTransitionType::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

  epedit = std::make_unique<EpBox::EditEyepieces>(app);

  scopeedit = std::make_unique<ScopeBox::EditTelescopes>(app);

  scopegrid.attach(scopeedit->create_telescope_grid(), 0, 0); // create extra grid containers 
  epgrid.attach(epedit->create_eyepiece_grid(), 0, 0);        // to keep GtkStack happy with child property_ids
  stack.add(scopegrid, "Edittelescopes", _("Edit telescopes"));                
  stack.add(epgrid, "Editeyepieces", _("Edit eyepieces"));
 

  switcher.set_hexpand(false);
  stack.set_vexpand(true);
  stack.set_hexpand(true);
  switcher.set_stack(stack);

  add(stack);
  show_all_children();
  epedit->init();
  scopeedit->init();

  signal_hide().connect([this]()
  {
    if (true == m_previouslogstate) AppGlobals::LOGFLAG = true;
  });
}

bool EquipWindow::on_key_press_event(GdkEventKey *key_event)
{

  if (key_event->keyval == GDK_KEY_Escape)
  {
    hide();
    return true;
  }

  return Gtk::Window::on_key_press_event(key_event);
}
 