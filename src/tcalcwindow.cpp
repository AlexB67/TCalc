#include <gtkmm/settings.h>
#include <glibmm/i18n.h>
#include <filesystem>
#include <iostream>
#include "tcalcwindow.hpp"
#include "gtkmmcustomutils.hpp"
#include "astrocalclib/astrocalc.hpp"
#include "appglobals.hpp"
#include "fileio.hpp"
#include <giomm/appinfo.h>

TcalcWindow::TcalcWindow(const Glib::RefPtr<Gtk::Application>& app)
    : Glib::ObjectBase("TcalcWindow"), Gtk::ApplicationWindow(), m_app(app)
{
   set_border_width(Uidefs::BORDER_WIDTH);
   fileIO::set_app_data();

   // Create separate grids to avoid expansion / layout issues

   windowgrid.set_column_spacing(Uidefs::COLUMN_PADDING);
   windowgrid.set_row_spacing(Uidefs::ROW_PADDING);
   gridleft.set_column_spacing(Uidefs::COLUMN_PADDING);
   gridleft.set_row_spacing(Uidefs::ROW_PADDING);
   gridright.set_column_spacing(Uidefs::COLUMN_PADDING);
   gridright.set_row_spacing(Uidefs::ROW_PADDING);

   logbox = std::make_unique<LogView::LoggerView>();
   scopebox = std::make_shared<ScopeBox::Telescopebox>();
   epbox = std::make_shared<EpBox::Eyepiecebox>();
   calcbox = std::make_unique<CalcBox::Calcbox>();
   magbox = std::make_unique<MagBox::Magbox>();
   optionsbox = std::make_unique<OptionsBox::Optionsbox>();
   resultsbox = std::make_unique<ResultsBox::Resultsbox>();

   gridleft.attach(scopebox->create_telescope_grid(), 0, 0, 1, 3);
   gridleft.attach(epbox->create_eyepiece_grid(), 1, 0, 1, 3);
   gridleft.attach(calcbox->create_calc_grid(), 0, 3, 2, 1);
   gridleft.attach(logbox->create_logger(), 0, 4, 2, 3);
   gridright.attach(magbox->create_mag_grid(), 0, 0, 1, 5);
   gridright.attach(optionsbox->create_options_grid(), 0, 5, 1, 2);
   gridright.attach(resultsbox->create_results_grid(), 2, 0, 1, 7);

   windowgrid.attach(gridleft, 0, 0);
   windowgrid.attach(gridright, 1, 0);

   add(windowgrid);

   set_headerbar();
   create_menu_and_shortcuts();
   set_signal_handlers();
   get_keyfile_settings();
   show_all_children();

   epbox->init();
   scopebox->init();
   magbox->set_default_mode();
   calcbox->m_calcbutton.grab_focus();

   AppGlobals::LOGFLAG = true;

   logbox->setlogtext(AppGlobals::LOGFLAG, LogView::tINFO, _("TCalc ready."));
   AppGlobals::app_notify(_("TCalc ready."), m_app);
}

void TcalcWindow::create_menu_and_shortcuts()
{
   add_action("about", sigc::mem_fun(*this, &TcalcWindow::about));
   add_action("quit", sigc::mem_fun(*this, &TcalcWindow::close));
   add_action("preferences", sigc::mem_fun(*this, &TcalcWindow::prefs));
   add_action("shortcuts", sigc::mem_fun(*this, &TcalcWindow::shortcuts));

   add_action("graphs", [this]() {
      if (!graphswindow)
         graphswindow = std::make_unique<GraphsWindow>();
      graphswindow->present();
   });

   add_action("ocular", [this]() {
      if (!ocularwindow)
         ocularwindow = std::make_unique<OcularWindow>();
      ocularwindow->present();
   });

   add_action("help", [this]() {
      Gio::AppInfo::launch_default_for_uri("help:tcalc");
   });

   add_action("calculate", [this]() {
      calcbox->m_calcbutton.activate();
   });

   add_action("clear", [this]() {
      calcbox->m_clearbutton.activate();
   });

   add_action("selectset1", [this]() {
      resultsbox->m_buttonleft.activate();
   });

   add_action("selectset2", [this]() {
      resultsbox->m_buttonright.activate();
   });

   add_action("interactive", [this]() {
      calcbox->m_interactive->activate();
   });

   add_action("logging", [this]() {
      calcbox->m_logswitch->activate();
   });

   add_action("linearmethod", [this]() {
      optionsbox->m_uselinearmethod->activate();
   });

   add_action("fstop", [this]() {
      optionsbox->m_uselinearmethod->activate();
   });

   add_action("dsomode", [this]() {
      magbox->set_dso_mode(true);
   });

   add_action("starmode", [this]() {
      magbox->set_dso_mode(false);
   });

   add_action("equipment", sigc::mem_fun(*this, &TcalcWindow::equipment));
   add_action("menu", [this]() { menubutton.set_active(true); });
   add_action("search", sigc::mem_fun(*this, &TcalcWindow::search));

   winmenu = Gio::Menu::create();
   winmenusection = Gio::Menu::create();

   winmenu->append(_("_Interactive graphs"), "win.graphs");
   winmenu->append(_("_Ocular viewer"), "win.ocular");
   winmenu->append(_("_Edit equipment"), "win.equipment");

   winmenusection->append(_("_Preferences"), "win.preferences");
   winmenusection->append(_("_Keyboard shortcuts"), "win.shortcuts");
   winmenusection->append(_("_Help"), "win.help");
   winmenusection->append(_("_About TCalc"), "win.about");
   winmenusection->append(_("_Quit"), "win.quit");

   winmenu->insert_section(3, winmenusection);
   menubutton.set_menu_model(winmenu);

   m_app->set_accel_for_action("win.quit", "<Ctrl>q");
   m_app->set_accel_for_action("win.menu", "<Alt>m");
   m_app->set_accel_for_action("win.search", "<Ctrl>f");
   m_app->set_accel_for_action("win.graphs", "<Ctrl>g");
   m_app->set_accel_for_action("win.ocular", "<Ctrl>o");
   m_app->set_accel_for_action("win.equipment", "<Ctrl>e");
   m_app->set_accel_for_action("win.preferences", "<Ctrl>p");
   m_app->set_accel_for_action("win.help", "F1");
   m_app->set_accel_for_action("win.shortcuts", "<Ctrl>k");
   m_app->set_accel_for_action("win.calculate", "<Alt>c");
   m_app->set_accel_for_action("win.clear", "<Alt>l");
   m_app->set_accel_for_action("win.selectset2", "<Alt>Right");
   m_app->set_accel_for_action("win.selectset1", "<Alt>Left");
   m_app->set_accel_for_action("win.interactive", "<Ctrl>i");
   m_app->set_accel_for_action("win.logging", "<Ctrl>l");
   m_app->set_accel_for_action("win.fstop", "<Ctrl>s");
   m_app->set_accel_for_action("win.linearmethod", "<Ctrl>m");
   m_app->set_accel_for_action("win.dsomode", "<Ctrl>Page_Down");
   m_app->set_accel_for_action("win.starmode","<Ctrl>Page_Up");
}

void TcalcWindow::set_headerbar()
{
   menubutton.set_image_from_icon_name("open-menu-symbolic", Gtk::ICON_SIZE_BUTTON, true);
   menubutton.set_tooltip_text(_("Opens the menu."));
   searchbutton.set_image_from_icon_name("edit-find-symbolic", Gtk::ICON_SIZE_BUTTON, true);
   searchbutton.set_tooltip_text(_("Search for an eyepiece or telescope model."));
   headerbar.set_title(_("GNOME TCalc"));
   headerbar.set_subtitle(_("An astronomy tool for telescopes and eyepieces."));
   headerbar.set_show_close_button();
   headerbar.pack_start(searchbutton);
   headerbar.pack_start(magbox->get_switcher_ref());
   headerbar.pack_end(menubutton);
   set_titlebar(headerbar);
}

void TcalcWindow::get_keyfile_settings()
{
   using namespace Astrocalc;

   if (false == std::filesystem::exists(AppGlobals::configpath.c_str())) // firt time user, provide build in defaults
   {
      Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme().set_value(true);
      logbox->set_log_timestamp(true);
      return;
   }

   Glib::KeyFile keyfile;

   try
   {
      keyfile.load_from_file(AppGlobals::configpath);
   }
   catch (const Glib::Error &ex)
   {
      std::cerr << "Error loading key file: " << ex.what() << '\n';
      return;
   }

   std::vector<bool> appearance = keyfile.get_boolean_list("Appearance", "settings");
   auto preferdarktheme = Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme();

   preferdarktheme.set_value(appearance[0]);
   logbox->set_log_timestamp(appearance[1]);
   logbox->set_log_colour(appearance[2]);
   logbox->set_use_monospace(appearance[3]);
   appearance.clear();

   std::vector<double> sdefaults = keyfile.get_double_list("Telescope optical defaults", "telescopes");
   astrocalc::SCOPEREFLECT = sdefaults[0];
   astrocalc::SCOPETRANS = sdefaults[1];
   astrocalc::OBSTRUCTSIZE = sdefaults[2];
   astrocalc::OBSTRUCTSIZESCT = sdefaults[3];
   sdefaults.clear();

   std::vector<double> edefaults = keyfile.get_double_list("Eyepiece optical defaults", "eyepieces");
   astrocalc::ETRANS = edefaults[0];
   astrocalc::ETRANSPLOSSL = edefaults[1];
   astrocalc::ETRANSORTHO = edefaults[2];
   edefaults.clear();
}

double TcalcWindow::calctFOV()
{
   using namespace Astrocalc;
   using namespace AppGlobals;
   astrocalc m_calc;
   short method;
   double dtFOV;

   if (true == optionsbox->m_uselinearmethod->get_active())
   {
      method = astrocalc::USESMALLANGLEMETHOD;
      logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Using linear method to calculate tFOV."));
   }
   else
   {
      method = astrocalc::USESTDMETHOD;
      logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Linear method disabled to calculate tFOV."));
   }

   if (true == optionsbox->m_usefstop->get_active())
   {
      dtFOV = m_calc.calc_tFOV(scopebox->m_sflen.get_value(), epbox->m_efstop.get_value());
      logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Using Field stop formula for tFOV calculation."));

      if (fabs(epbox->m_efstop.get_value()) < Astrocalc::astrocalc::tSMALL)
      {
         logbox->setlogtext(LOGFLAG, LogView::tWARN, _("Field stop formula used, but field stop incorrect"));
         logbox->setlogtext(LOGFLAG, LogView::tWARN, _("Check field stop data or disable field stop method."));
      }

      if (Astrocalc::astrocalc::USESTDMETHOD == method)
         dtFOV = dtFOV * m_calc.calc_kfactor(epbox->m_efov.get_value()) / epbox->m_efov.get_value();
   }
   else
   {
      dtFOV = m_calc.calc_tFOV(epbox->m_efov.get_value(), scopebox->m_sflen.get_value(), epbox->m_eflen.get_value(), method);

      if (fabs(epbox->m_efstop.get_value()) < Astrocalc::astrocalc::tSMALL)
         logbox->setlogtext(LOGFLAG, LogView::tINFO, _("Field stop data unknown."));
      else
         logbox->setlogtext(LOGFLAG, LogView::tWARN, _("Field stop data ignored."));
   }

   return dtFOV / astrocalc::DEGTOARCMIN;
}
