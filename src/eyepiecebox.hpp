#pragma once
#include "eyepiececombomodel.hpp"
#include "gtkmmcustomutils.hpp"
#include "loggerbox.hpp"
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <glibmm/i18n.h>

namespace EpBox
{
class Eyepiecebox
{

public:
  explicit Eyepiecebox(const bool userdataonly = false);
  virtual ~Eyepiecebox() {}
  Eyepiecebox(const Eyepiecebox &) = delete;
  Eyepiecebox(Eyepiecebox&& ) = delete;
  Eyepiecebox &operator=(const Eyepiecebox &other) = delete;
  Eyepiecebox &operator=(Eyepiecebox&& ) = delete;

  Gtk::Frame &create_eyepiece_grid();
  virtual void init() { ep_changed(); } // call this afer all widgets are shown
  virtual void set_default_values();
  void enable_fstop(const bool is_enabled) { m_efstop.set_sensitive(is_enabled); }
  Gtk::ComboBox *m_emodel;
  EpCombo::EpCombomodel m_ecombomodel;
  Gtk::ComboBoxText m_etype;
  Ui::SpinEntry m_efov;
  Ui::SpinEntry m_eflen;
  Ui::SpinEntry m_efstop;
  Ui::SpinEntry m_erelief;
  Ui::SpinEntry m_etrans;

private:
  Gtk::Label m_efovlabel{_("FOV/degrees"), Gtk::Align::START};
  Gtk::Label m_eflenlabel{_("Focal length/mm"), Gtk::Align::START};
  Gtk::Label m_efstoplabel{_("Field stop/mm"), Gtk::Align::START};
  Gtk::Label m_erelieflabel{_("Eye relief/mm"), Gtk::Align::START};
  Gtk::Label m_etranslabel{_("Transmission/%"), Gtk::Align::START};
  Gtk::Label m_etypelabel{_("Eyepiece type"), Gtk::Align::START};
  int get_eyepiece_type(const Gtk::TreeModel::Row &row) const;
  bool m_userdataonly;

protected:
  Gtk::Grid m_grid;
  Gtk::Frame  m_frame;
  Gtk::Label  m_framelabel;
  Gtk::Label  m_emodellabel{_("Select eyepiece"), Gtk::Align::CENTER};
  void ep_changed();
  virtual void create_epmodel_connection();
};
} // namespace EpBox
