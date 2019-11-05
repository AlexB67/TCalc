#include <iostream>
#include <cairommconfig.h>
#include <fstream>
#include <filesystem>
#include "ocularwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include "fileio.hpp"
#include "astrocalc.hpp"
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
  scopebox->frame_can_expand(true);

  create_sky_objects_box();

  controlsgrid.attach(epbox->create_eyepiece_grid(), 0, 0);
  controlsgrid.attach(scopebox->create_telescope_grid(), 0, 1);
  controlsgrid2.attach(dsoframe, 0, 0);
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

  fileIO::dbfileIO db;
	db.load_dso_data(dsocombo, m_dsocombomodel);

  ocularbox.m_efov = epbox->m_efov.get_value();

  add(windowgrid);
  set_signal_handlers();
  optionsbox->show_wavelength(false);
  
  show_all_children();

  epbox->init();
  scopebox->init();
  dso_changed();
  dsocombo.grab_focus();
 // test();
}

void OcularWindow::test()
{
  double eyepupil = 7.5;
  double nelm = 6.5;
 // double minsize = 1.7;
 // double maxsize = 2.2;
  double minsize = 23;
  double maxsize = 23;
  double ap = 254;
  double tr = 0.7;
 // double objmag = 12.5;
  double objmag = 5.8;

  Astrocalc::astrocalc m;

  double objbright = m.calc_dso_mag_to_brightness(objmag, minsize, maxsize);

  double magn[] = {40.0, 50.0, 60.0 , 70.0, 90.0, 100.0, 120, 150, 180};
  double darkening[9];
  double axis[9];
  double logc[9];

  double bsky = m.calc_nelm_brightness_threshold_method(nelm);

   // 0.1116 = sqrt (transmission) / eyepupil
  

  std::clog << "obj brightness " << objbright << std::endl;
  std::clog << "sky darkness naked eye " << bsky << std::endl;
  std::clog << "obj contrast index " << m.calc_contrast_index(bsky, objbright) << std::endl <<  std::endl;


  for (auto i = 0; i < 9; ++i)
  {
    darkening[i] = - 5.0 * log10((sqrt(tr) / eyepupil) * (ap / magn[i]));
    axis[i] = log10(magn[i] * minsize);
    logc[i] = m.calc_thrconcs(magn[i] * minsize, bsky + darkening[i]);
    std::clog << "darkening at mag " << magn[i] << "  " <<  darkening[i] << " in scope " << bsky + darkening[i] << " logaxis " << axis[i] 
  << " logcontrast " << log10(logc[i]) <<  " Contrast : " << m.calc_contrast_index(bsky, objbright) - log10(logc[i]) << std::endl;
  }

}

void OcularWindow::create_sky_objects_box()
{
  dsoframe.set_label_widget(dsoframelabel);
  dsoframelabel.set_markup(_("<b>DSO objects</b>"));
  dsoframe.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

  Uidefs::set_ui_spacing<Gtk::Grid>(dsoobjgrid);

  skyobjectlabel.set_label(_("Select object"));
  minoraxislabel.set_markup(_("Minor axis/arcmin"));
  majoraxislabel.set_markup(_("Major axis/arcmin"));
  dsotypelabel.set_label(_("DSO type"));
  dsocontrastlabel.set_markup(_("Object contrast"));
  vmaglabel.set_markup(_("DSO magnitude"));
  nelmlabel.set_markup(_("NELM"));
  skyscopelabel.set_markup(_("Sky telescope/mag sec<sup>-2</sup>"));
  surfacebrightnesslabel.set_markup(_("DSO Brightness/mag sec<sup>-2</sup>"));
  skybglabel.set_markup(_("Sky brightness/mag sec<sup>-2</sup>"));
  obscontrastlabel.set_label(_("Observed contrast"));
  optmaglabel.set_label(_("Optimal magnification"));

  skyobjectlabel.set_halign(Gtk::ALIGN_START);
  minoraxislabel.set_halign(Gtk::ALIGN_START);
  majoraxislabel.set_halign(Gtk::ALIGN_START);
  vmaglabel.set_halign(Gtk::ALIGN_START);
  dsotypelabel.set_halign(Gtk::ALIGN_START);
  surfacebrightnesslabel.set_halign(Gtk::ALIGN_START);
  dsocontrastlabel.set_halign(Gtk::ALIGN_START);
  nelmlabel.set_halign(Gtk::ALIGN_START);
  skybglabel.set_halign(Gtk::ALIGN_START);
  skyscopelabel.set_halign(Gtk::ALIGN_START);
  obscontrastlabel.set_halign(Gtk::ALIGN_START);
  optmaglabel.set_halign(Gtk::ALIGN_START);

  minoraxis.set_spin_entry(1.0, 1.0, 10000, 0.1, 2, true);
  minoraxis.set_tooltip_text(_("Represented as an ellipse, this is the minor axis in arc min."));
  majoraxis.set_spin_entry(1.0, 1.0, 10000, 0.1, 2, true);
  majoraxis.set_tooltip_text(_("Represented as an ellipse, this is the major axis in arc min.."));
  dsotype.set_tooltip_text(("The type of DSO object, galaxy, nebula etc."));
  dsotype.set_can_focus(false);
  skyscope.set_can_focus(false);
  obscontrast.set_can_focus(false);
  optmag.set_can_focus(false);
  vmag.set_spin_entry(1.0, 1.0, 25, 0.1, 2, true);
  vmag.set_tooltip_text(_("The apparent magnitude of the object."));
  nelm.set_spin_entry(6.5, 1.0, 25, 0.025, 2, true);
  surfacebrightness.set_spin_entry(1.0, 1.0, 100, 0.01, 2, true);
  surfacebrightness.set_tooltip_text(_("The apparent surface brightness of the object."));
  skybglabel.set_tooltip_text(_("The background brightness of the sky."));
  dsocontrast.set_spin_entry(0.0, -100, 100, 0.01, 2, true);
  skybg.set_spin_entry(0.0, 0, 100, 0.1, 2, true);
  

  dsoobjgrid.attach(skyobjectlabel, 0, 0);
  dsoobjgrid.attach(dsocombo, 1, 0);
  dsoobjgrid.attach(dsotypelabel, 0, 1);
  dsoobjgrid.attach(dsotype, 1, 1);
  dsoobjgrid.attach(minoraxislabel, 0, 2);
  dsoobjgrid.attach(minoraxis, 1, 2);
  dsoobjgrid.attach(majoraxislabel, 0, 3);
  dsoobjgrid.attach(majoraxis, 1, 3);
  dsoobjgrid.attach(vmaglabel, 0, 4);
  dsoobjgrid.attach(vmag, 1, 4);
  dsoobjgrid.attach(surfacebrightnesslabel, 0, 5);
  dsoobjgrid.attach(surfacebrightness, 1, 5);
  dsoobjgrid.attach(dsocontrastlabel, 0, 6);
  dsoobjgrid.attach(dsocontrast, 1, 6);
  dsoobjgrid.attach(nelmlabel, 0, 7);
  dsoobjgrid.attach(nelm, 1, 7);
  dsoobjgrid.attach(skybglabel, 0, 8);
  dsoobjgrid.attach(skybg, 1, 8);
  dsoobjgrid.attach(skyscopelabel, 0, 9);
  dsoobjgrid.attach(skyscope, 1, 9);
  dsoobjgrid.attach(obscontrastlabel, 0, 10);
  dsoobjgrid.attach(obscontrast, 1, 10);
  dsoobjgrid.attach(optmaglabel, 0, 11);
  dsoobjgrid.attach(optmag, 1, 11);


  AppGlobals::get_keyfile_config(dsoframe);
  AppGlobals::set_frame_style.connect([this](){ AppGlobals::change_frame_style(dsoframe);});
  
  dsoframe.add(dsoobjgrid);
}

void OcularWindow::create_ocular_info_box()
{
  ocularfovlabel.set_label(_("FOV : "));
  ocularexitpupillabel.set_label(_("Exit pupil : "));
  ocularmaglabel.set_label(_("Magnification : "));
  ocularfov.set_width_chars(12);
  ocularfov.set_editable(false);
  ocularfov.set_can_focus(false);
  ocularexitpupil.set_width_chars(12);
  ocularexitpupil.set_editable(false);
  ocularexitpupil.set_can_focus(false);
  ocularmag.set_width_chars(12);
  ocularmag.set_editable(false);
  ocularmag.set_can_focus(false);

  Uidefs::set_ui_spacing(ocularinfogrid);
  ocularinfogrid.set_halign(Gtk::ALIGN_CENTER);

  ocularinfogrid.attach(ocularfovlabel, 0, 0);
  ocularinfogrid.attach(ocularfov, 1, 0);
  ocularinfogrid.attach(ocularexitpupillabel, 2, 0);
  ocularinfogrid.attach(ocularexitpupil, 3, 0);
  ocularinfogrid.attach(ocularmaglabel, 4, 0);
  ocularinfogrid.attach(ocularmag, 5, 0);
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
