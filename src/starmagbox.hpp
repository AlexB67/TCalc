#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/scale.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/entry.h>
#include <gtkmm/sizegroup.h>
#include <gtkmm/separator.h>
#include <glibmm/i18n.h>
#include "astrocalclib/astrocalc.hpp"
#include "gtkmmcustomutils.hpp"
#include "appglobals.hpp"
#include "loggerbox.hpp"
#include "dsocombomodel.hpp"
#include "glibmmcustomutils.hpp"

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
    Gtk::StackSwitcher &get_switcher_ref(){ return m_switcher;}

    void set_default_mode() { m_selectdsolabel.set_visible(false); m_dsocombo.set_visible(false);
                  m_dsotypelabel.set_visible(false); m_dsotype.set_visible(false);}
    
    void set_dso_mode(const bool mode){ (true == mode ) ? m_stack.set_visible_child(m_dsogrid) 
                                                        : m_stack.set_visible_child(m_stargrid);}
    
    void set_stack_transition_time(const gint animation_time){m_stack.set_transition_duration(animation_time);};
    void set_ocular_mode();

    Gtk::ComboBoxText   m_sitetype;
    Gtk::ComboBoxText   m_sitetype1;
    Gtk::ComboBoxText   m_dirtlevel;
    Gtk::ComboBoxText   m_colourbyname;
    Gtk::ComboBox     m_dsocombo;
    Ui::SpinEntry     m_vmag;
    Ui::SpinEntry     m_bgsky;
    Ui::SpinEntry		  m_minoraxis;
	  Ui::SpinEntry		  m_majoraxis;
    Ui::SpinEntry		  m_dsobrightness;
    Ui::SpinEntry     m_dsocontrastindex;
    Ui::SpinEntry		  m_age;
	  Ui::SpinEntry		  m_pupilsize;
    Ui::SpinEntry		  m_nelm;
    Ui::SpinEntry		  m_nelm1;
    Ui::SpinEntry		  m_sqm;
	  Ui::SpinEntry		  m_colour;
    Ui::SpinEntry     m_seeing;
    Ui::SpinEntry     m_extinction;
    Ui::SpinEntry     m_zenith;
    Gtk::Entry        m_dsotype;
    Gtk::Scale        m_explevel;
    DSOCombomodel     m_dsocombomodel;
    
  private:
    Gtk::Separator      m_sep;
    Gtk::StackSwitcher  m_switcher;
    Gtk::Stack          m_stack;       
    Gtk::Label          m_minoraxislabel;
    Gtk::Label          m_majoraxislabel;
    Gtk::Label          m_vmaglabel;
    Gtk::Label          m_dsocontrastindexlabel;
    Gtk::Label          m_selectdsolabel;
    Gtk::Label          m_dsotypelabel;
    Gtk::Label          m_bgskylabel;
    Gtk::Label          m_dsobrightnesslabel;
    Gtk::Label          m_dsocontrastlabel;
    Gtk::Label			    m_dirtlevellabel;
	  Gtk::Label			    m_agelabel;
    Gtk::Label          m_pupillabel;
    Gtk::Label          m_zenithlabel;
	  Gtk::Label			    m_colourlabel;
    Gtk::Label          m_colouradjustlabel;
    Gtk::Label          m_seeinglabel;
    Gtk::Label          m_explevellabel;
    Gtk::Label          m_nelmlabel;
    Gtk::Label          m_nelmlabel1;
    Gtk::Label          m_sqmlabel;
    Gtk::Label          m_extinctionlabel;
    Gtk::Label          m_sitetypelabel;
    Gtk::Label          m_sitetypelabel1;
    Gtk::Label          m_framelabel;
    Gtk::Label          m_dsoframelabel;
	  Gtk::Grid    		    m_grid;
    Gtk::Grid           m_stargrid;
    Gtk::Grid           m_maingrid;
    Gtk::Grid           m_dsogrid;
    Gtk::Frame          m_frame;
    Astrocalc::astrocalc  m_calc;
    const std::vector<double> sitevalues = {6.0, 7.8, 7.3, 6.8, 6.3, 5.8, 5.3, 4.8, 4.3, 4.0};
    void set_signal_handlers();
    
  protected:
    void site_type_changed();
    void dso_site_type_changed();
    void nelm_changed();
    void sqm_changed();
    void nelm1_changed();
    void sqm1_changed();
    void dso_brightness_changhed();
    void star_colour_changed();
    void dso_changed();

    
    sigc::connection nelm_connection;
    sigc::connection sqm_connection;
    sigc::connection nelm1_connection;
    sigc::connection sqm1_connection;
  };
}
