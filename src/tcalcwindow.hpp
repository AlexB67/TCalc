#pragma once
#include <gtkmm/applicationwindow.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/shortcutswindow.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/frame.h>
#include <gtkmm/textview.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/button.h>
#include <gtkmm/aboutdialog.h>
#include <memory.h>
#include "telescopebox.hpp"
#include "eyepiecebox.hpp"
#include "loggerbox.hpp"
#include "calcbox.hpp"
#include "starmagbox.hpp"
#include "optionsbox.hpp"
#include "resultsbox.hpp"
#include "prefswindow.hpp"
#include "graphswindow.hpp"
#include "ocularwindow.hpp"
#include "equipwindow.hpp"
#include "browse_equipwindow.hpp"

class TcalcWindow : public Gtk::ApplicationWindow
{
public:
  explicit TcalcWindow(const Glib::RefPtr<Gtk::Application>& app);
  TcalcWindow(const TcalcWindow&) = delete;
  TcalcWindow(TcalcWindow&& ) = delete;
  TcalcWindow& operator=(const TcalcWindow& other) = delete;
  TcalcWindow& operator=(TcalcWindow&& other) = delete;
  virtual ~TcalcWindow(){ /*close(); */}


private:
  Glib::RefPtr<Gtk::Application> m_app;
  Gtk::ScrolledWindow mainwin;
  std::unique_ptr<PrefsWindow> prefswindow;
  std::unique_ptr<EquipWindow> equipwindow;
  std::unique_ptr<GraphsWindow> graphswindow;
  std::unique_ptr<OcularWindow> ocularwindow;
  std::unique_ptr<BrowserWindow> equipbrowserwindow;
  std::shared_ptr<ScopeBox::Telescopebox> scopebox;
  std::shared_ptr<EpBox::Eyepiecebox> epbox;
  std::unique_ptr<LogView::LoggerView> logbox;
  std::unique_ptr<CalcBox::Calcbox> calcbox;
  std::unique_ptr<MagBox::Magbox> magbox;
  std::unique_ptr<OptionsBox::Optionsbox> optionsbox;
  std::unique_ptr<ResultsBox::Resultsbox> resultsbox;
  std::unique_ptr<Gtk::AboutDialog> aboutdialog;
  Gtk::ShortcutsWindow *shortcutswindow = nullptr;
  Gtk::Label  title_label;
  Gtk::Grid   windowgrid;
  Gtk::Grid   gridright;
  Gtk::Grid   gridleft;
  Gtk::HeaderBar  headerbar;
  Gtk::MenuButton menubutton;
  Glib::RefPtr<Gio::Menu> winmenu;
  Glib::RefPtr<Gio::Menu> winmenusection;
  Gtk::Settings *settheme;

  void set_signal_handlers();
  void create_menu_and_shortcuts();
  void set_headerbar();
  void create_results();
  void get_keyfile_settings();
  double calctFOV();

protected:
  void about();
  void prefs();
  void equipment();
  void shortcuts();
  void close();
  
};
