#include <iostream>
#include <cairommconfig.h>
#include <fstream>
#include <filesystem>
#include "ocularwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include "fileio.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "appglobals.hpp"

OcularWindow::OcularWindow()
{
  searchbutton.set_image_from_icon_name("edit-find-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  searchbutton.set_tooltip_text(_("Search for an eyepiece or telescope model."));
  headerbar.set_title(_("Ocular viewer"));
  headerbar.set_subtitle(_("An astronomy tool for telescopes and eyepieces."));
  headerbar.set_show_close_button();
  nightmode = Gtk::make_managed<Gtk::Switch>();
  nightmode->set_active(false);
  nightmode->set_valign(Gtk::ALIGN_CENTER);
  nightmode->set_halign(Gtk::ALIGN_CENTER);
  nightmode->set_tooltip_text(_("Switch to dark mode."));
  headerbar.pack_end(*nightmode);
  headerbar.pack_start(searchbutton);
  set_titlebar(headerbar);

  provider = Gtk::CssProvider::create();
  epbox = std::make_shared<EpBox::Eyepiecebox>();
  scopebox = std::make_shared<ScopeBox::Telescopebox>();
  optionsbox = std::make_unique<OptionsBox::Optionsbox>();
  magbox = std::make_unique<MagBox::Magbox>();
  scopebox->frame_can_expand(true);

  controlsgrid.attach(epbox->create_eyepiece_grid(), 0, 0);
  controlsgrid.attach(scopebox->create_telescope_grid(), 0, 1);
  controlsgrid2.attach(magbox->create_mag_grid(), 0, 0);
  controlsgrid2.attach(optionsbox->create_options_grid(), 0, 1);

  ocularboxframe.set_label_widget(ocularboxframelabel);
  ocularboxframelabel.set_markup(_("<b>Ocular View</b>"));
  ocularboxframe.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
  ocularboxframe.set_hexpand(true);
  ocularboxframe.set_vexpand(true);
  ocularboxframe.set_border_width(Uidefs::BORDER_WIDTH);
  AppGlobals::get_keyfile_config(ocularboxframe);
  AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(ocularboxframe);});

  ocularframe.set_shadow_type(Gtk::SHADOW_NONE); // frame to maintain aspect ratio for the ocular
  ocularframe.set_halign(Gtk::ALIGN_FILL);
  ocularframe.set_valign(Gtk::ALIGN_FILL);
  ocularframe.set_hexpand(true);
  ocularframe.set_vexpand(true);

  ocularframe.add(ocularbox);
  create_ocular_info_box();

  ocularboxgrid.attach(ocularframe, 0, 0);
  ocularboxgrid.attach(ocularinfogrid, 0, 1);

  ocularboxframe.add(ocularboxgrid);
  
  oculargrid.set_hexpand(true);
  oculargrid.set_vexpand(true);

  windowgrid.attach(ocularboxframe, 0, 0);
  windowgrid.attach(controlsgrid, 1, 0);
  windowgrid.attach(controlsgrid2, 2, 0);

  Uidefs::set_ui_spacing<Gtk::Grid>(oculargrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid);
  Uidefs::set_ui_spacing<Gtk::Grid>(controlsgrid2);

  ocularbox.m_efov = epbox->m_efov.get_value();

  add(windowgrid);
  set_signal_handlers();
  optionsbox->show_wavelength(false);
  
  show_all_children();

  epbox->init();
  scopebox->init();
  magbox->set_dso_mode(true);
  magbox->set_ocular_mode();
  dso_changed();
  magbox->m_dsocombo.grab_focus();
}

void OcularWindow::create_ocular_info_box()
{
  ocularfovlabel.set_markup(_("<i>θ</i><sub>t</sub>"));
  ocularexitpupillabel.set_markup(_("<i>D</i><sub>e</sub>"));
  ocularmaglabel.set_markup(_("<i>M</i><sub>c</sub>"));
  ocularfov.set_width_chars(16);
  ocularfov.set_editable(false);
  ocularfov.set_can_focus(false);
  ocularexitpupil.set_width_chars(7);
  ocularexitpupil.set_editable(false);
  ocularexitpupil.set_can_focus(false);
  ocularmag.set_width_chars(7);
  obscontrast.set_max_width_chars(7);
  ocularmag.set_editable(false);
  ocularmag.set_can_focus(false);
  obscontrastlabel.set_markup(_("log(<i>C</i>/<i>C</i><sub>o</sub>)"));

  Uidefs::set_ui_spacing(ocularinfogrid);
  ocularinfogrid.set_halign(Gtk::ALIGN_CENTER);

  ocularinfogrid.attach(ocularfovlabel, 0, 0);
  ocularinfogrid.attach(ocularfov, 1, 0);
  ocularinfogrid.attach(ocularexitpupillabel, 2, 0);
  ocularinfogrid.attach(ocularexitpupil, 3, 0);
  ocularinfogrid.attach(ocularmaglabel, 4, 0);
  ocularinfogrid.attach(ocularmag, 5, 0);
  ocularinfogrid.attach(obscontrastlabel, 6, 0);
  ocularinfogrid.attach(obscontrast, 7, 0);

}

bool OcularWindow::on_key_press_event(GdkEventKey *key_event)
{

  if (key_event->keyval == GDK_KEY_Escape)
  {
    hide();
    return true;
  }
  else if (key_event->keyval == GDK_KEY_F11)
  {
    (get_screen()->get_height() == get_height()) ? unfullscreen() :  fullscreen();
    
    return true;
  }
  else if ((key_event->keyval == GDK_KEY_F10))
  {
    if (false == nightmode->get_active()) nightmode->set_active(true); else nightmode->set_active(false);
    return true;
  }
  else if ((key_event->keyval == GDK_KEY_f) && (key_event->state & GDK_CONTROL_MASK))
  {
    search();
    return true;
  }


  return Gtk::Window::on_key_press_event(key_event);
}

OcularWindow::~OcularWindow()
{
  if (searchwindow)
  {
    searchwindow->hide();
  }
  hide();
}
