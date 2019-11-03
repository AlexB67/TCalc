#pragma once
#include <gtkmm/spinbutton.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <glibmm/i18n.h>
#include "gtkmmcustomutils.hpp"

namespace OptionsBox
{
  class Optionsbox
  {
  public:
    explicit Optionsbox();
    Optionsbox(const Optionsbox&) = delete;
    Optionsbox& operator=(const Optionsbox& other) = delete;
    Gtk::Frame &create_options_grid();
    void set_default_values();
    void show_wavelength(bool show);
    Gtk::Switch         *m_usefstop;
    Gtk::Switch         *m_uselinearmethod;
    Ui::SpinEntry		    m_wavelength;
    
  private:
    Gtk::Label			    m_usefstoplabel{_("Use fieldstop"), Gtk::ALIGN_START};
    Gtk::Label          m_uselinearmethodlabel{_("Linear method"), Gtk::ALIGN_START};
    Gtk::Label          m_wavelengthlabel{_("Wavelength/nm"), Gtk::ALIGN_START};
    Gtk::Label          m_framelabel;
	  Gtk::Grid    		    m_grid;
    Gtk::Frame          m_frame;

    
protected:
    //signals
    
  };
}
