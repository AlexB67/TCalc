#pragma once
#include "telescopecombomodel.hpp"
#include "loggerbox.hpp"
#include "gtkmmcustomutils.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/widget.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/combobox.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/sizegroup.h>
#include <glibmm/i18n.h>

namespace ScopeBox
{
class Telescopebox
{

public:
  explicit Telescopebox(const bool userdataonly = false);
  Telescopebox(const Telescopebox &) = delete;
  Telescopebox(Telescopebox &&) = delete;
  Telescopebox &operator=(const Telescopebox &other) = delete;
  Telescopebox &operator=(Telescopebox &&) = delete;
  virtual ~Telescopebox() {}

  Gtk::Frame &create_telescope_grid();
  virtual void init() { scope_changed(); } // call this afer all widgets are shown
  virtual void set_default_values();
  void frame_can_expand(const bool expand) { m_frame.set_vexpand(expand); }

  Gtk::ComboBox m_smodel;
  ScopeCombo::ScopeCombomodel m_scombomodel;
  Gtk::ComboBoxText m_stype;
  Ui::SpinEntry m_sflen;
  Ui::SpinEntry m_saperture;
  Ui::SpinEntry m_sfratio;
  Ui::SpinEntry m_sreflect;
  Ui::SpinEntry m_sobstruct;

private:
  Gtk::Label m_sflenlabel{_("Focal length/mm"), Gtk::ALIGN_START};
  Gtk::Label m_saperturelabel{_("Aperture/mm"), Gtk::ALIGN_START};
  Gtk::Label m_sfratiolabel{_("Focal ratio"), Gtk::ALIGN_START};
  Gtk::Label m_sreflectlabel{_("Reflectivity/%"), Gtk::ALIGN_START};
  Gtk::Label m_sobstructlabel{_("Obstruction/%"), Gtk::ALIGN_START};
  Gtk::Label m_stypelabel{_("Telescope type"), Gtk::ALIGN_START};
  bool m_userdataonly;

protected:
  Gtk::Grid m_grid;
  Gtk::Frame m_frame;
  Gtk::Label m_framelabel;
  Gtk::Label m_smodellabel{_("Select telescope"), Gtk::ALIGN_CENTER};
  void update_sfratio();
  void set_sflen();
  void scope_changed();
  void scope_type_changed();
  virtual void create_scopemodel_connection();
};
} // namespace ScopeBox
