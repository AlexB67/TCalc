#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/scale.h>
#include <glibmm/i18n.h>
#include "astrocalc.hpp"
#include "gtkmmcustomutils.hpp"
#include "appglobals.hpp"
#include "loggerbox.hpp"

namespace MagBox
{
  class Magbox
  {
    
  public:
    explicit Magbox();
    Magbox(const Magbox&) = delete;
    Magbox& operator=(const Magbox& other) = delete;

    Gtk::Frame &create_mag_grid();
    void set_default_values();
    double get_optical_dirt_level() const;

    Gtk::ComboBoxText   m_sitetype;
    Gtk::ComboBoxText   m_dirtlevel;
    Gtk::ComboBoxText   m_colourbyname;
    Ui::SpinEntry		  m_age;
	  Ui::SpinEntry		  m_pupilsize;
    Ui::SpinEntry		  m_nelm;
    Ui::SpinEntry		  m_sqm;
	  Ui::SpinEntry		  m_colour;
    Ui::SpinEntry     m_seeing;
    Ui::SpinEntry     m_extinction;
    Ui::SpinEntry     m_zenith;
    Gtk::Scale        m_explevel;
    
  private:
    Gtk::Label			    m_dirtlevellabel;
	  Gtk::Label			    m_agelabel;
    Gtk::Label          m_pupillabel;
    Gtk::Label          m_zenithlabel;
	  Gtk::Label			    m_colourlabel;
    Gtk::Label          m_colouradjustlabel;
    Gtk::Label          m_seeinglabel;
    Gtk::Label          m_explevellabel;
    Gtk::Label          m_nelmlabel;
    Gtk::Label          m_sqmlabel;
    Gtk::Label          m_extinctionlabel;
    Gtk::Label          m_sitetypelabel;
    Gtk::Label          m_framelabel;
	  Gtk::Grid    		    m_grid;
    Gtk::Frame          m_frame;
    Astrocalc::astrocalc  m_calc;
    const std::vector<double> sitevalues = {6.0, 7.8, 7.3, 6.8, 6.3, 5.8, 5.3, 4.8, 4.3, 4.0};
    void set_signal_handlers();
    
  protected:
    void site_type_changed();
    void nelm_changed();
    void sqm_changed();
    void star_colour_changed();
    
    sigc::connection nelm_connection;
    sigc::connection sqm_connection;
  };
}
