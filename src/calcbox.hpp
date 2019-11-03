#pragma once
#include <gtkmm/button.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/separator.h>
#include <gtkmm/frame.h>
#include <gtkmm/sizegroup.h>
#include <glibmm/i18n.h>
#include "loggerbox.hpp"

namespace CalcBox
{
  class Calcbox
  {
  public:
    explicit Calcbox();
    Calcbox(const Calcbox&) = delete;
    Calcbox& operator=(const Calcbox& other) = delete;
    Gtk::Frame &create_calc_grid();
    void set_default_values();
    
    Gtk::Button         m_clearbutton{_("Clear")};
    Gtk::Button         m_calcbutton{_("Calculate")};
    Gtk::Switch         *m_logswitch;
    Gtk::Switch         *m_interactive;

  private:     
    Gtk::Label			m_loglabel{_("Logging"), Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER};
    Gtk::Label          m_interactivelabel{_("Interactive"), Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER};
    Gtk::Label          m_calcframelabel;    
	Gtk::Grid    		m_calcgrid;
    Gtk::Frame          m_calcframe;
    Gtk::Separator      m_separator;
    Gtk::Separator      m_separator2;
    Gtk::Separator      m_separator3;
    Glib::RefPtr<Gtk::SizeGroup>  m_sizegroup;
  };
}
